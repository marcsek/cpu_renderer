#include "triangle_rasterizer.h"
#include "app.h"
#include "essentials.h"
#include "pipeline.h"
#include "shaders/vertex.h"
#include "z_buffer.h"
#include <math.h>

static void draw_flat(renderer *rn, const vertex *v1, const vertex *v2,
                      const vertex *v3, const vertex *dv1, const vertex *dv2,
                      vertex it_edge1, const pipeline *p);
static void draw_flat_top(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, const pipeline *p);
static void draw_flat_bot(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, const pipeline *p);

void triangle_rasterizer_draw(renderer *rn, triangle tr, const pipeline *p) {
  const vertex *v1 = &tr.v0;
  const vertex *v2 = &tr.v1;
  const vertex *v3 = &tr.v2;

  if (v2->pos.y < v1->pos.y)
    nstd_swap((void **)&v1, (void **)&v2);
  if (v3->pos.y < v2->pos.y)
    nstd_swap((void **)&v3, (void **)&v2);
  if (v2->pos.y < v1->pos.y)
    nstd_swap((void **)&v2, (void **)&v1);

  if (v1->pos.y == v2->pos.y) {
    if (v2->pos.x < v1->pos.x)
      nstd_swap((void **)&v1, (void **)&v2);
    draw_flat_top(rn, v1, v2, v3, p);
  } else if (v3->pos.y == v2->pos.y) {
    if (v3->pos.x < v2->pos.x)
      nstd_swap((void **)&v2, (void **)&v3);
    draw_flat_bot(rn, v1, v2, v3, p);
  } else {
    const float alphaSplit = (v2->pos.y - v1->pos.y) / (v3->pos.y - v1->pos.y);
    vertex vi = vertex_interpolate_to(v1, v3, alphaSplit);

    if (v2->pos.x < vi.pos.x) {
      draw_flat_bot(rn, v1, v2, &vi, p);
      draw_flat_top(rn, v2, &vi, v3, p);
    } else {
      draw_flat_bot(rn, v1, &vi, v2, p);
      draw_flat_top(rn, &vi, v2, v3, p);
    }
    vertex_free(&vi);
  }
}

static void draw_flat_top(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, const pipeline *p) {
  const float delta_y = v3->pos.y - v1->pos.y;

  vertex dv0 = vertex_copy(v3);
  vertex_sub(&dv0, v1);
  vertex_div(&dv0, delta_y);

  vertex dv1 = vertex_copy(v3);
  vertex_sub(&dv1, v2);
  vertex_div(&dv1, delta_y);

  vertex v2_c = vertex_copy(v2);

  draw_flat(rn, v1, v2, v3, &dv0, &dv1, v2_c, p);

  vertex_free(&dv0);
  vertex_free(&dv1);
  vertex_free(&v2_c);
}

static void draw_flat_bot(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, const pipeline *p) {
  const float delta_y = v3->pos.y - v1->pos.y;

  vertex dv0 = vertex_copy(v2);
  vertex_sub(&dv0, v1);
  vertex_div(&dv0, delta_y);

  vertex dv1 = vertex_copy(v3);
  vertex_sub(&dv1, v1);
  vertex_div(&dv1, delta_y);

  vertex v1_c = vertex_copy(v1);

  draw_flat(rn, v1, v2, v3, &dv0, &dv1, v1_c, p);

  vertex_free(&dv0);
  vertex_free(&dv1);
  vertex_free(&v1_c);
}

static void draw_flat(renderer *rn, const vertex *v1, const vertex *v2,
                      const vertex *v3, const vertex *dv1, const vertex *dv2,

                      vertex it_edge1, const pipeline *p) {
  UNUSED(v2);
  vertex it_edge0 = vertex_copy(v1);

  const int y_start = MAX((int)ceil((double)(v1->pos.y - 0.5f)), 0);
  const int y_end =
      MIN((int)ceil((double)(v3->pos.y - 0.5f)), WINDOW_HEIGHT - 1);

  vertex dv1_c = vertex_copy(dv1);
  vertex_mult(&dv1_c, ((float)y_start) + 0.5f - v1->pos.y);
  vertex_add(&it_edge0, &dv1_c);

  vertex dv2_c = vertex_copy(dv2);
  vertex_mult(&dv2_c, ((float)y_start) + 0.5f - v1->pos.y);
  vertex_add(&it_edge1, &dv2_c);

  for (int y = y_start; y < y_end;
       y++, vertex_add(&it_edge0, dv1), vertex_add(&it_edge1, dv2)) {

    const int x_start = MAX((int)ceil((double)(it_edge0.pos.x - 0.5f)), 0);
    const int x_end =
        MIN((int)ceil(((double)it_edge1.pos.x - 0.5f)), WINDOW_WIDTH - 1);

    vertex i_line = vertex_copy(&it_edge0);

    const float dx = it_edge1.pos.x - it_edge0.pos.x;
    vertex di_line = vertex_copy(&it_edge1);
    vertex_sub(&di_line, &i_line);
    vertex_div(&di_line, dx);

    vertex di_line_c = vertex_copy(&di_line);
    vertex_mult(&di_line_c, ((float)x_start) + 0.5f - it_edge0.pos.x);
    vertex_add(&i_line, &di_line_c);

    for (int x = x_start; x < x_end; x++, vertex_add(&i_line, &di_line)) {
      if (z_buffer_test_and_set((z_buffer *)p->zb, x, y, i_line.pos.z)) {
        const float w = 1.0f / i_line.pos.w;
        vertex i_line_c = vertex_copy(&i_line);
        vertex_mult(&i_line_c, w);

        renderer_put_pixel(
            rn, x, y,
            p->effect.ps.create_pixel(p->effect.ps.shader_data, &i_line_c));

        vertex_free(&i_line_c);
      }
    }
    vertex_free(&i_line);
    vertex_free(&di_line);
    vertex_free(&di_line_c);
  }
  vertex_free(&dv1_c);
  vertex_free(&dv2_c);
  vertex_free(&it_edge0);
}
