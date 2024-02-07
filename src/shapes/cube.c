#include "../texture-vertex.h"
#include "shapes.h"
#include "vector.h"
#include <notstd.h>
#include <stdbool.h>
#include <stdlib.h>

static vec3 v_buffer[8];
static vec2 tc_buffer[8];
static tex_vertex tv_buffer[8];
static int i_buffer[24] = {0, 1, 1, 3, 3, 2, 2, 0, 0, 4, 1, 5,
                           3, 7, 2, 6, 4, 5, 5, 7, 7, 6, 6, 4};

static int t_buffer[36] = {0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5,
                           2, 6, 3, 3, 6, 7, 4, 5, 7, 4, 7, 6,
                           0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4};

static indexed_line_list get_lines(shape *sh) {
  return (indexed_line_list){
      .vertices = sh->vertices,
      .indices = i_buffer,
  };
}

static indexed_triangle_list get_triangles(shape *sh) {
  return (indexed_triangle_list){.vertices = shape_copy_verts(sh->vertices),
                                 .indices = t_buffer,
                                 .cull_flags = calloc(12, sizeof(bool))};
}

static indexed_triangle_list_tex get_triangles_tex(struct shape *sh) {
  return (indexed_triangle_list_tex){
      .vertices = shape_copy_verts_tex(sh->tv),
      .indices = t_buffer,
      .cull_flags = calloc(12, sizeof(bool)),
  };
}

shape cube_create(float size) {
  const float side = size / 2.0f;

  vector *vertices = vector_create(8);
  vector *tv = vector_create(8);
  v_buffer[0] = (vec3){-side, -side, -side};
  tc_buffer[0] = (vec2){0.0f, 1.0f};
  tv_buffer[0] = (tex_vertex){.tc = tc_buffer[0], .pos = v_buffer[0]};
  v_buffer[1] = (vec3){side, -side, -side};
  tc_buffer[1] = (vec2){1.0f, 1.0f};
  tv_buffer[1] = (tex_vertex){.tc = tc_buffer[1], .pos = v_buffer[1]};
  v_buffer[2] = (vec3){-side, side, -side};
  tc_buffer[2] = (vec2){0.0f, 0.0f};
  tv_buffer[2] = (tex_vertex){.tc = tc_buffer[2], .pos = v_buffer[2]};
  v_buffer[3] = (vec3){side, side, -side};
  tc_buffer[3] = (vec2){1.0f, 0.0f};
  tv_buffer[3] = (tex_vertex){.tc = tc_buffer[3], .pos = v_buffer[3]};
  v_buffer[4] = (vec3){-side, -side, side};
  tc_buffer[4] = (vec2){1.0f, 1.0f};
  tv_buffer[4] = (tex_vertex){.tc = tc_buffer[4], .pos = v_buffer[4]};
  v_buffer[5] = (vec3){side, -side, side};
  tc_buffer[5] = (vec2){0.0f, 1.0f};
  tv_buffer[5] = (tex_vertex){.tc = tc_buffer[5], .pos = v_buffer[5]};
  v_buffer[6] = (vec3){-side, side, side};
  tc_buffer[6] = (vec2){1.0f, 0.0f};
  tv_buffer[6] = (tex_vertex){.tc = tc_buffer[6], .pos = v_buffer[6]};
  v_buffer[7] = (vec3){side, side, side};
  tc_buffer[7] = (vec2){0.0f, 0.0f};
  tv_buffer[7] = (tex_vertex){.tc = tc_buffer[7], .pos = v_buffer[7]};

  for (size_t i = 0; i < 8; i++) {
    vector_push_back(vertices, &v_buffer[i]);
    vector_push_back(tv, &tv_buffer[i]);
  }
  return (shape){
      .tv = tv,
      .vertices = vertices,
      .get_lines = get_lines,
      .get_triangles = get_triangles,
      .get_triangles_tex = get_triangles_tex,
  };
}
