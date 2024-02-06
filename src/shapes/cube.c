#include "shapes.h"
#include "vector.h"
#include <notstd.h>

static vec3 v_buffer[8];
static int i_buffer[24] = {0, 1, 1, 3, 3, 2, 2, 0, 0, 4, 1, 5,
                           3, 7, 2, 6, 4, 5, 5, 7, 7, 6, 6, 4};

static int t_buffer[36] = {0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5,
                           2, 6, 3, 3, 6, 7, 4, 5, 7, 4, 7, 6,
                           0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4};
static bool f_buffer[12];

cube cube_create(float size) {
  const float side = size / 2.0f;

  vector *vertices = vector_create(8);
  v_buffer[0] = (vec3){-side, -side, -side};
  v_buffer[1] = (vec3){side, -side, -side};
  v_buffer[2] = (vec3){-side, side, -side};
  v_buffer[3] = (vec3){side, side, -side};
  v_buffer[4] = (vec3){-side, -side, side};
  v_buffer[5] = (vec3){side, -side, side};
  v_buffer[6] = (vec3){-side, side, side};
  v_buffer[7] = (vec3){side, side, side};

  for (size_t i = 0; i < 8; i++) {
    vector_push_back(vertices, &v_buffer[i]);
  }
  return (cube){.vertices = vertices};
}

indexed_line_list cube_get_lines(cube *c) {
  return (indexed_line_list){
      .vertices = c->vertices,
      .indices = i_buffer,
  };
}

indexed_triangle_list cube_get_triangles(cube *c) {
  return (indexed_triangle_list){
      .vertices = c->vertices, .indices = t_buffer, .cull_flags = f_buffer};
}
