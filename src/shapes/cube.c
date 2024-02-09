#include "../shaders/vertex.h"
#include "shapes.h"
#include "vector.h"
#include <notstd.h>
#include <stdbool.h>

static vec3 v_buffer[14];
static vec2 tc_buffer[14];
static vertex tv_buffer[14];
static int i_buffer[24] = {0, 1, 1, 3, 3, 2, 2, 0, 0, 4, 1, 5,
                           3, 7, 2, 6, 4, 5, 5, 7, 7, 6, 6, 4};

static int t_buffer[36] = {0, 2,  1,  2, 3,  1, 4,  8, 5, 5,  8, 9,
                           2, 6,  3,  3, 6,  7, 4,  5, 7, 4,  7, 6,
                           2, 10, 11, 2, 11, 6, 12, 3, 7, 12, 7, 13};

static indexed_triangle_list get_skinned(shape *sh) {
  return (indexed_triangle_list){
      .vertices = sh->vertices,
      .indices = t_buffer,
  };
}

static vec2 convert_tex_coords(float u, float v) {
  return (vec2){(u + 1.0f) / 3.0f, v / 4.0f};
}

shape cube_create(float size) {
  const float side = size / 2.0f;

  vector *tv = vector_create(14);
  v_buffer[0] = (vec3){-side, -side, -side};
  tc_buffer[0] = convert_tex_coords(1.0f, 0.0f);

  v_buffer[1] = (vec3){side, -side, -side};
  tc_buffer[1] = convert_tex_coords(0.0f, 0.0f);

  v_buffer[2] = (vec3){-side, side, -side};
  tc_buffer[2] = convert_tex_coords(1.0f, 1.0f);

  v_buffer[3] = (vec3){side, side, -side};
  tc_buffer[3] = convert_tex_coords(0.0f, 1.0f);

  v_buffer[4] = (vec3){-side, -side, side};
  tc_buffer[4] = convert_tex_coords(1.0f, 3.0f);

  v_buffer[5] = (vec3){side, -side, side};
  tc_buffer[5] = convert_tex_coords(0.0f, 3.0f);

  v_buffer[6] = (vec3){-side, side, side};
  tc_buffer[6] = convert_tex_coords(1.0f, 2.0f);

  v_buffer[7] = (vec3){side, side, side};
  tc_buffer[7] = convert_tex_coords(0.0f, 2.0f);

  v_buffer[8] = (vec3){-side, -side, -side};
  tc_buffer[8] = convert_tex_coords(1.0f, 4.0f);

  v_buffer[9] = (vec3){side, -side, -side};
  tc_buffer[9] = convert_tex_coords(0.0f, 4.0f);

  v_buffer[10] = (vec3){-side, -side, -side};
  tc_buffer[10] = convert_tex_coords(2.0f, 1.0f);

  v_buffer[11] = (vec3){-side, -side, side};
  tc_buffer[11] = convert_tex_coords(2.0f, 2.0f);

  v_buffer[12] = (vec3){side, -side, -side};
  tc_buffer[12] = convert_tex_coords(-1.0f, 1.0f);

  v_buffer[13] = (vec3){side, -side, side};
  tc_buffer[13] = convert_tex_coords(-1.0f, 2.0f);

  for (size_t i = 0; i < 14; i++) {
    tv_buffer[i] = tex_vertex_create(v_buffer[i], &tc_buffer[i]);
    vector_push_back(tv, &tv_buffer[i]);
  }

  return (shape){.get_skinned = get_skinned, .vertices = tv};
}
