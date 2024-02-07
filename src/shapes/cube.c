#include "../texture_vertex.h"
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
      .vertices = sh->tex_verts,
      .indices = i_buffer,
  };
}

static indexed_triangle_list get_triangles(shape *sh) {
  return (indexed_triangle_list){.vertices =
                                     shape_copy_verts_tex(sh->tex_verts),
                                 .indices = t_buffer,
                                 .cull_flags = calloc(12, sizeof(bool))};
}

static indexed_triangle_list_tex get_triangles_tex(struct shape *sh) {
  return (indexed_triangle_list_tex){
      .vertices = shape_copy_verts_tex(sh->tex_verts),
      .indices = t_buffer,
      .cull_flags = calloc(12, sizeof(bool)),
  };
}

shape cube_create(float size) {
  const float side = size / 2.0f;

  vector *tv = vector_create(8);
  v_buffer[0] = (vec3){-side, -side, -side};
  tc_buffer[0] = (vec2){0.0f, 1.0f};
  v_buffer[1] = (vec3){side, -side, -side};
  tc_buffer[1] = (vec2){1.0f, 1.0f};
  v_buffer[2] = (vec3){-side, side, -side};
  tc_buffer[2] = (vec2){0.0f, 0.0f};
  v_buffer[3] = (vec3){side, side, -side};
  tc_buffer[3] = (vec2){1.0f, 0.0f};
  v_buffer[4] = (vec3){-side, -side, side};
  tc_buffer[4] = (vec2){1.0f, 1.0f};
  v_buffer[5] = (vec3){side, -side, side};
  tc_buffer[5] = (vec2){0.0f, 1.0f};
  v_buffer[6] = (vec3){-side, side, side};
  tc_buffer[6] = (vec2){1.0f, 0.0f};
  v_buffer[7] = (vec3){side, side, side};
  tc_buffer[7] = (vec2){0.0f, 0.0f};

  for (size_t i = 0; i < 8; i++) {
    tv_buffer[i] = (tex_vertex){.pos = v_buffer[i], .tc = tc_buffer[i]};
    vector_push_back(tv, &tv_buffer[i]);
  }
  return (shape){
      .tex_verts = tv,
      .get_lines = get_lines,
      .get_triangles = get_triangles,
      .get_triangles_tex = get_triangles_tex,
  };
}
