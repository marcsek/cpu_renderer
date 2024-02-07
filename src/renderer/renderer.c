#include "renderer.h"
#include "vec2.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void renderer_create_flat_top_triangle(renderer *rn, const vec2 *v1,
                                              const vec2 *v2, const vec2 *v3,
                                              uint32_t v);
static void renderer_create_flat_bot_triangle(renderer *rn, const vec2 *v1,
                                              const vec2 *v2, const vec2 *v3,
                                              uint32_t v);
static void renderer_create_flat_top_triangle_tex(renderer *rn,
                                                  const tex_vertex *v1,
                                                  const tex_vertex *v2,
                                                  const tex_vertex *v3,
                                                  surface *sf);

static void renderer_create_flat_bot_triangle_tex(renderer *rn,
                                                  const tex_vertex *v1,
                                                  const tex_vertex *v2,
                                                  const tex_vertex *v3,
                                                  surface *sf);

static uint32_t *base_buffer = NULL;

renderer renderer_create(size_t buffer_width, size_t buffer_height) {
  size_t buffer_length = buffer_width * buffer_height;

  uint32_t *buffer = (uint32_t *)calloc(buffer_length, sizeof(uint32_t));
  base_buffer = (uint32_t *)calloc(buffer_length, sizeof(uint32_t));

  renderer rn = (renderer){
      ._buffer = buffer,
      .buffer_width = buffer_width,
      .buffer_height = buffer_height,
      .buffer_size = buffer_length *= sizeof(uint32_t),
  };
  return rn;
}

uint32_t *renderer_get_buffer(renderer *rn) { return rn->_buffer; }

void renderer_put_pixel(renderer *rn, unsigned int x, unsigned int y,
                        uint32_t v) {
  assert(x >= 0 && x < rn->buffer_width);
  assert(y >= 0 && y < rn->buffer_height);
  rn->_buffer[rn->buffer_width * y + x] = v;
}

void renderer_reset_buffer(renderer *rn) {
  memcpy(rn->_buffer, base_buffer, rn->buffer_size);
}

uint32_t *renderer_destroy(renderer *rn) {
  uint32_t *buffer = rn->_buffer;

  free(base_buffer);
  base_buffer = NULL;
  free(rn);

  return buffer;
}

void renderer_create_line(renderer *rn, int x1, int y1, int x2, int y2,
                          uint32_t v) {
  float m = 0.0f;
  if (x1 != x2) {
    m = (float)(y2 - y1) / (x2 - x1);
  }

  if (x1 != x2 && fabs(m) <= 1.0f) {
    if (x1 > x2) {
      int tmpx = x1;
      int tmpy = y1;
      x1 = x2;
      x2 = tmpx;
      y1 = y2;
      y2 = tmpy;
    }

    float b = y1 - m * x1;

    for (int x = x1; x < x2; x++) {
      int y = (int)(m * x + b);

      if (y >= 0 && y < rn->buffer_height && x >= 0 && x < rn->buffer_width) {
        renderer_put_pixel(rn, x, y, v);
      }
    }
  } else {
    if (y1 > y2) {
      int tmpx = x1;
      int tmpy = y1;
      x1 = x2;
      x2 = tmpx;
      y1 = y2;
      y2 = tmpy;
    }

    float w = (float)(x2 - x1) / (y2 - y1);
    float p = x1 - w * y1;

    for (int y = y1; y < y2; y++) {
      int x = (int)(w * y + p);
      if (y >= 0 && y < rn->buffer_height && x >= 0 && x < rn->buffer_width) {
        renderer_put_pixel(rn, x, y, v);
      }
    }
  }
}

void renderer_create_triangle(renderer *rn, const vec2 *v1, const vec2 *v2,
                              const vec2 *v3, uint32_t v) {
  if (v2->y < v1->y)
    nstd_swap((void **)&v1, (void **)&v2);
  if (v3->y < v2->y)
    nstd_swap((void **)&v3, (void **)&v2);
  if (v2->y < v1->y)
    nstd_swap((void **)&v2, (void **)&v1);

  if (v1->y == v2->y) {
    if (v2->x < v1->x)
      nstd_swap((void **)&v1, (void **)&v2);
    renderer_create_flat_top_triangle(rn, v1, v2, v3, v);
  } else if (v3->y == v2->y) {
    if (v3->x < v2->x)
      nstd_swap((void **)&v2, (void **)&v3);
    renderer_create_flat_bot_triangle(rn, v1, v2, v3, v);
  } else {
    const float alphaSplit = (v2->y - v1->y) / (v3->y - v1->y);
    vec2 vi = vec2_copy(v3);
    vec2_subs(&vi, v1);
    vec2_mult_s(&vi, alphaSplit);
    vec2_add(&vi, v1);

    if (v2->x < vi.x) {
      renderer_create_flat_bot_triangle(rn, v1, v2, &vi, v);
      renderer_create_flat_top_triangle(rn, v2, &vi, v3, v);
    } else {
      renderer_create_flat_bot_triangle(rn, v1, &vi, v2, v);
      renderer_create_flat_top_triangle(rn, &vi, v2, v3, v);
    }
  }
}

void renderer_create_triangle_tex(renderer *rn, const tex_vertex *v1,
                                  const tex_vertex *v2, const tex_vertex *v3,
                                  surface *sf) {
  if (v2->pos.y < v1->pos.y) {
    nstd_swap((void **)&v1, (void **)&v2);
  }
  if (v3->pos.y < v2->pos.y)
    nstd_swap((void **)&v3, (void **)&v2);
  if (v2->pos.y < v1->pos.y)
    nstd_swap((void **)&v2, (void **)&v1);

  if (v1->pos.y == v2->pos.y) {
    if (v2->pos.x < v1->pos.x)
      nstd_swap((void **)&v1, (void **)&v2);
    renderer_create_flat_top_triangle_tex(rn, v1, v2, v3, sf);
  } else if (v3->pos.y == v2->pos.y) {
    if (v3->pos.x < v2->pos.x)
      nstd_swap((void **)&v2, (void **)&v3);
    renderer_create_flat_bot_triangle_tex(rn, v1, v2, v3, sf);
  } else {
    const float alphaSplit = (v2->pos.y - v1->pos.y) / (v3->pos.y - v1->pos.y);
    const tex_vertex vi = tex_ver_interpolate_to(v1, v3, alphaSplit);

    if (v2->pos.x < vi.pos.x) {
      renderer_create_flat_bot_triangle_tex(rn, v1, v2, &vi, sf);
      renderer_create_flat_top_triangle_tex(rn, v2, &vi, v3, sf);
    } else {
      renderer_create_flat_bot_triangle_tex(rn, v1, &vi, v2, sf);
      renderer_create_flat_top_triangle_tex(rn, &vi, v2, v3, sf);
    }
  }
}

static void renderer_create_flat_top_triangle(renderer *rn, const vec2 *v1,
                                              const vec2 *v2, const vec2 *v3,
                                              uint32_t v) {
  float m0 = (v3->x - v1->x) / (v3->y - v1->y);
  float m1 = (v3->x - v2->x) / (v3->y - v2->y);

  const int y_start = (int)ceil((double)(v1->y - 0.5f));
  const int y_end = (int)ceil((double)(v3->y - 0.5f));

  for (int y = y_start; y < y_end; y++) {
    const float px0 = m0 * (((float)y) + 0.5f - v1->y) + v1->x;
    const float px1 = m1 * (((float)y) + 0.5f - v2->y) + v2->x;

    const int x_start = (int)ceil((double)(px0 - 0.5f));
    const int x_end = (int)ceil((double)(px1 - 0.5f));

    for (int x = x_start; x < x_end; x++) {
      renderer_put_pixel(rn, x, y, v);
    }
  }
}

static void renderer_create_flat_bot_triangle(renderer *rn, const vec2 *v1,
                                              const vec2 *v2, const vec2 *v3,
                                              uint32_t v) {
  float m0 = (v2->x - v1->x) / (v2->y - v1->y);
  float m1 = (v3->x - v1->x) / (v3->y - v1->y);

  const int y_start = (int)ceil((double)(v1->y - 0.5f));
  const int y_end = (int)ceil((double)(v3->y - 0.5f));

  for (int y = y_start; y < y_end; y++) {
    const float px0 = m0 * (((float)y) + 0.5f - v1->y) + v1->x;
    const float px1 = m1 * (((float)y) + 0.5f - v1->y) + v1->x;

    const int x_start = (int)ceil((double)(px0 - 0.5f));
    const int x_end = (int)ceil(((double)px1 - 0.5f));

    for (int x = x_start; x < x_end; x++) {
      renderer_put_pixel(rn, x, y, v);
    }
  }
}

static void
renderer_create_flat_triangle_tex(renderer *rn, const tex_vertex *v1,
                                  const tex_vertex *v2, const tex_vertex *v3,
                                  surface *sf, const tex_vertex *dv1,
                                  const tex_vertex *dv2, tex_vertex it_edge1) {
  tex_vertex it_edge0 = *v1;

  const int y_start = (int)ceil((double)(v1->pos.y - 0.5f));
  const int y_end = (int)ceil((double)(v3->pos.y - 0.5f));

  tex_vertex dv1_c = tex_ver_copy(dv1);
  tex_ver_mult(&dv1_c, ((float)y_start) + 0.5f - v1->pos.y);
  tex_ver_add(&it_edge0, &dv1_c);

  tex_vertex dv2_c = tex_ver_copy(dv2);
  tex_ver_mult(&dv2_c, ((float)y_start) + 0.5f - v1->pos.y);
  tex_ver_add(&it_edge1, &dv2_c);

  const float tex_width = (float)sf->width;
  const float tex_heigh = (float)sf->height;
  const float tex_clamp_x = tex_width - 1.0f;
  const float tex_clamp_y = tex_heigh - 1.0f;

  for (int y = y_start; y < y_end;
       y++, tex_ver_add(&it_edge0, dv1), tex_ver_add(&it_edge1, dv2)) {

    const int x_start = (int)ceil((double)(it_edge0.pos.x - 0.5f));
    const int x_end = (int)ceil(((double)it_edge1.pos.x - 0.5f));

    vec2 tc_edge_diff = vec2_copy(&it_edge1.tc);
    vec2_subs(&tc_edge_diff, &it_edge0.tc);
    vec2_div_s(&tc_edge_diff, it_edge1.pos.x - it_edge0.pos.x);

    const vec2 dtc_line = vec2_copy(&tc_edge_diff);

    vec2 dtc_line_c = vec2_copy(&dtc_line);
    vec2_mult_s(&dtc_line_c, ((float)x_start) + 0.5f - it_edge0.pos.x);

    vec2 itc_line = vec2_copy(&it_edge0.tc);
    vec2_add(&itc_line, &dtc_line_c);

    for (int x = x_start; x < x_end; x++, vec2_add(&itc_line, &dtc_line)) {
      renderer_put_pixel(
          rn, x, y,
          surface_get_pixel(sf, (int)MIN(itc_line.x * tex_width, tex_clamp_x),
                            (int)MIN(itc_line.y * tex_heigh, tex_clamp_y)));
    }
  }
}

static void renderer_create_flat_top_triangle_tex(renderer *rn,
                                                  const tex_vertex *v1,
                                                  const tex_vertex *v2,
                                                  const tex_vertex *v3,
                                                  surface *sf) {
  const float delta_y = v3->pos.y - v1->pos.y;

  tex_vertex dv0 = tex_ver_copy(v3);
  tex_ver_sub(&dv0, v1);
  tex_ver_div(&dv0, delta_y);

  tex_vertex dv1 = tex_ver_copy(v3);
  tex_ver_sub(&dv1, v2);
  tex_ver_div(&dv1, delta_y);

  renderer_create_flat_triangle_tex(rn, v1, v2, v3, sf, &dv0, &dv1,
                                    tex_ver_copy(v2));
}

static void renderer_create_flat_bot_triangle_tex(renderer *rn,
                                                  const tex_vertex *v1,
                                                  const tex_vertex *v2,
                                                  const tex_vertex *v3,
                                                  surface *sf) {
  const float delta_y = v3->pos.y - v1->pos.y;

  tex_vertex dv0 = tex_ver_copy(v2);
  tex_ver_sub(&dv0, v1);
  tex_ver_div(&dv0, delta_y);

  tex_vertex dv1 = tex_ver_copy(v3);
  tex_ver_sub(&dv1, v1);
  tex_ver_div(&dv1, delta_y);

  renderer_create_flat_triangle_tex(rn, v1, v2, v3, sf, &dv0, &dv1,
                                    tex_ver_copy(v1));
}
