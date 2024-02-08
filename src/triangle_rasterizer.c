#include "triangle_rasterizer.h"
#include "helpers/debug_info.h"
#include "vertex.h"
#include <math.h>

static void draw_flat(renderer *rn, const vertex *v1, const vertex *v2,
                      const vertex *v3, const vertex *dv1, const vertex *dv2,
                      vertex it_edge1, surface *tex);
static void draw_flat_top(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, surface *tex);
static void draw_flat_bot(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, surface *tex);

void triangle_rasterizer_draw(renderer *rn, triangle tr, surface *tex) {
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
    draw_flat_top(rn, v1, v2, v3, tex);
  } else if (v3->pos.y == v2->pos.y) {
    if (v3->pos.x < v2->pos.x)
      nstd_swap((void **)&v2, (void **)&v3);
    draw_flat_bot(rn, v1, v2, v3, tex);
  } else {
    const float alphaSplit = (v2->pos.y - v1->pos.y) / (v3->pos.y - v1->pos.y);
    const vertex vi = vertex_interpolate_to(v1, v3, alphaSplit);

    if (v2->pos.x < vi.pos.x) {
      draw_flat_bot(rn, v1, v2, &vi, tex);
      draw_flat_top(rn, v2, &vi, v3, tex);
    } else {
      draw_flat_bot(rn, v1, &vi, v2, tex);
      draw_flat_top(rn, &vi, v2, v3, tex);
    }
  }
}

static void draw_flat_top(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, surface *tex) {
  const float delta_y = v3->pos.y - v1->pos.y;

  vertex dv0 = vertex_copy(v3);
  vertex_sub(&dv0, v1);
  vertex_div(&dv0, delta_y);

  vertex dv1 = vertex_copy(v3);
  vertex_sub(&dv1, v2);
  vertex_div(&dv1, delta_y);

  draw_flat(rn, v1, v2, v3, &dv0, &dv1, vertex_copy(v2), tex);
}

static void draw_flat_bot(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, surface *tex) {
  const float delta_y = v3->pos.y - v1->pos.y;

  vertex dv0 = vertex_copy(v2);
  vertex_sub(&dv0, v1);
  vertex_div(&dv0, delta_y);

  vertex dv1 = vertex_copy(v3);
  vertex_sub(&dv1, v1);
  vertex_div(&dv1, delta_y);

  draw_flat(rn, v1, v2, v3, &dv0, &dv1, vertex_copy(v1), tex);
}

static void draw_flat(renderer *rn, const vertex *v1, const vertex *v2,
                      const vertex *v3, const vertex *dv1, const vertex *dv2,
                      vertex it_edge1, surface *tex) {
  vertex it_edge0 = *v1;

  const int y_start = (int)ceil((double)(v1->pos.y - 0.5f));
  const int y_end = (int)ceil((double)(v3->pos.y - 0.5f));

  vertex dv1_c = vertex_copy(dv1);
  vertex_mult(&dv1_c, ((float)y_start) + 0.5f - v1->pos.y);
  vertex_add(&it_edge0, &dv1_c);

  vertex dv2_c = vertex_copy(dv2);
  vertex_mult(&dv2_c, ((float)y_start) + 0.5f - v1->pos.y);
  vertex_add(&it_edge1, &dv2_c);

  const float tex_width = (float)tex->width;
  const float tex_height = (float)tex->height;
  const float tex_clamp_x = tex_width - 1.0f;
  const float tex_clamp_y = tex_height - 1.0f;

  for (int y = y_start; y < y_end;
       y++, vertex_add(&it_edge0, dv1), vertex_add(&it_edge1, dv2)) {

    const int x_start = (int)ceil((double)(it_edge0.pos.x - 0.5f));
    const int x_end = (int)ceil(((double)it_edge1.pos.x - 0.5f));

    vertex i_line = vertex_copy(&it_edge0);

    const float dx = it_edge1.pos.x - it_edge0.pos.x;
    vertex di_line = vertex_copy(&it_edge1);
    vertex_sub(&di_line, &i_line);
    vertex_div(&di_line, dx);

    vertex di_line_c = vertex_copy(&di_line);
    vertex_mult(&di_line_c, ((float)x_start) + 0.5f - it_edge0.pos.x);
    vertex_add(&i_line, &di_line_c);

    for (int x = x_start; x < x_end; x++, vertex_add(&i_line, &di_line)) {
      renderer_put_pixel(
          rn, x, y,
          surface_get_pixel(
              tex, (int)MIN(i_line.tc.x * tex_width + 0.5f, tex_clamp_x),
              (int)MIN(i_line.tc.y * tex_height + 0.5f, tex_clamp_y)));
    }
  }
}
