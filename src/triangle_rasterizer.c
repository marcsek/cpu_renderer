#include "triangle_rasterizer.h"
#include "pipeline.h"
#include "shaders/vertex.h"
#include <math.h>
#include <stdio.h>
;
static void draw_flat(renderer *rn, const vertex *v1, const vertex *v2,
                      const vertex *v3, const vertex *dv1, const vertex *dv2,
                      vertex it_edge1, surface *tex, const pipeline *p);
static void draw_flat_top(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, surface *tex, const pipeline *p);
static void draw_flat_bot(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, surface *tex, const pipeline *p);

void triangle_rasterizer_draw(renderer *rn, triangle tr, surface *tex,
                              const pipeline *p) {
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
    draw_flat_top(rn, v1, v2, v3, tex, p);
  } else if (v3->pos.y == v2->pos.y) {
    if (v3->pos.x < v2->pos.x)
      nstd_swap((void **)&v2, (void **)&v3);
    draw_flat_bot(rn, v1, v2, v3, tex, p);
  } else {
    const float alphaSplit = (v2->pos.y - v1->pos.y) / (v3->pos.y - v1->pos.y);
    vertex vi = v1->interpolate_to(v1, v3, alphaSplit);

    if (v2->pos.x < vi.pos.x) {
      draw_flat_bot(rn, v1, v2, &vi, tex, p);
      draw_flat_top(rn, v2, &vi, v3, tex, p);
    } else {
      draw_flat_bot(rn, v1, &vi, v2, tex, p);
      draw_flat_top(rn, &vi, v2, v3, tex, p);
    }
    vi.free(&vi);
  }
}

static void draw_flat_top(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, surface *tex, const pipeline *p) {
  const float delta_y = v3->pos.y - v1->pos.y;

  vertex dv0 = v3->copy(v3);
  v1->sub(&dv0, v1);
  dv0.div(&dv0, delta_y);

  vertex dv1 = v1->copy(v3);
  v2->sub(&dv1, v2);
  dv1.div(&dv1, delta_y);

  vertex v2_c = v2->copy(v2);

  draw_flat(rn, v1, v2, v3, &dv0, &dv1, v2_c, tex, p);

  dv0.free(&dv0);
  dv1.free(&dv1);
  v2_c.free(&v2_c);
}

static void draw_flat_bot(renderer *rn, const vertex *v1, const vertex *v2,
                          const vertex *v3, surface *tex, const pipeline *p) {
  const float delta_y = v3->pos.y - v1->pos.y;

  vertex dv0 = v2->copy(v2);
  dv0.sub(&dv0, v1);
  dv0.div(&dv0, delta_y);

  vertex dv1 = v3->copy(v3);
  dv1.sub(&dv1, v1);
  dv1.div(&dv1, delta_y);

  vertex v1_c = v1->copy(v1);

  draw_flat(rn, v1, v2, v3, &dv0, &dv1, v1_c, tex, p);
  dv0.free(&dv0);
  dv1.free(&dv1);
  v1_c.free(&v1_c);
}

static void draw_flat(renderer *rn, const vertex *v1, const vertex *v2,
                      const vertex *v3, const vertex *dv1, const vertex *dv2,
                      vertex it_edge1, surface *tex, const pipeline *p) {
  vertex it_edge0 = v1->copy(v1);

  const int y_start = (int)ceil((double)(v1->pos.y - 0.5f));
  const int y_end = (int)ceil((double)(v3->pos.y - 0.5f));

  vertex dv1_c = dv1->copy(dv1);
  dv1_c.mult(&dv1_c, ((float)y_start) + 0.5f - v1->pos.y);
  dv1_c.add(&it_edge0, &dv1_c);

  vertex dv2_c = dv2->copy(dv2);
  dv2_c.mult(&dv2_c, ((float)y_start) + 0.5f - v1->pos.y);
  dv2_c.add(&it_edge1, &dv2_c);

  for (int y = y_start; y < y_end;
       y++, it_edge0.add(&it_edge0, dv1), it_edge1.add(&it_edge1, dv2)) {

    const int x_start = (int)ceil((double)(it_edge0.pos.x - 0.5f));
    const int x_end = (int)ceil(((double)it_edge1.pos.x - 0.5f));

    vertex i_line = it_edge0.copy(&it_edge0);

    const float dx = it_edge1.pos.x - it_edge0.pos.x;
    vertex di_line = it_edge1.copy(&it_edge1);
    di_line.sub(&di_line, &i_line);
    di_line.div(&di_line, dx);

    vertex di_line_c = di_line.copy(&di_line);
    di_line_c.mult(&di_line_c, ((float)x_start) + 0.5f - it_edge0.pos.x);
    i_line.add(&i_line, &di_line_c);

    for (int x = x_start; x < x_end; x++, i_line.add(&i_line, &di_line)) {
      const float z = 1.0f / i_line.pos.z;
      vertex i_line_c = i_line.copy(&i_line);
      i_line_c.mult(&i_line_c, z);

      renderer_put_pixel(
          rn, x, y,
          p->effect.ps.create_pixel(p->effect.ps.shader_data, &i_line_c));

      i_line_c.free(&i_line_c);
    }
    i_line.free(&i_line);
    di_line.free(&di_line);
    di_line_c.free(&di_line_c);
  }
  dv1_c.free(&dv1_c);
  dv2_c.free(&dv2_c);
  it_edge0.free(&it_edge0);
}
