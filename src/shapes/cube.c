#include "../shaders/vertex.h"
#include "shapes.h"
#include "vector.h"
#include <notstd.h>
#include <stdbool.h>
#include <stdlib.h>

static int t_buffer[36] = {0, 2,  1,  2, 3,  1, 4,  8, 5, 5,  8, 9,
                           2, 6,  3,  3, 6,  7, 4,  5, 7, 4,  7, 6,
                           2, 10, 11, 2, 11, 6, 12, 3, 7, 12, 7, 13};

static int t_buffer_p[36] = {0, 2, 1, 2, 3, 1, 1, 3, 5, 3, 7, 5,
                             2, 6, 3, 3, 6, 7, 4, 5, 7, 4, 7, 6,
                             0, 4, 2, 2, 4, 6, 0, 1, 4, 1, 5, 4};

static vec2 convert_tex_coords(float u, float v) {
  return (vec2){(u + 1.0f) / 3.0f, v / 4.0f};
}

indexed_triangle_list cube_create_skinned(float size, vertex_create_func vcf) {
  const float side = size / 2.0f;

  vector *v = vector_create(14);
  vector *t = vector_create(14);

  for (size_t i = 0; i < 14; i++) {
    vector_push_back(v, malloc(sizeof(vec3)));
    vector_push_back(t, malloc(sizeof(vec2)));
  }

  vec3 **v_data = (vec3 **)vector_get_data(v);
  vec2 **t_data = (vec2 **)vector_get_data(t);

  *v_data[0] = (vec3){-side, -side, -side};
  *t_data[0] = convert_tex_coords(1.0f, 0.0f);

  *v_data[1] = (vec3){side, -side, -side};
  *t_data[1] = convert_tex_coords(0.0f, 0.0f);

  *v_data[2] = (vec3){-side, side, -side};
  *t_data[2] = convert_tex_coords(1.0f, 1.0f);

  *v_data[3] = (vec3){side, side, -side};
  *t_data[3] = convert_tex_coords(0.0f, 1.0f);

  *v_data[4] = (vec3){-side, -side, side};
  *t_data[4] = convert_tex_coords(1.0f, 3.0f);

  *v_data[5] = (vec3){side, -side, side};
  *t_data[5] = convert_tex_coords(0.0f, 3.0f);

  *v_data[6] = (vec3){-side, side, side};
  *t_data[6] = convert_tex_coords(1.0f, 2.0f);

  *v_data[7] = (vec3){side, side, side};
  *t_data[7] = convert_tex_coords(0.0f, 2.0f);

  *v_data[8] = (vec3){-side, -side, -side};
  *t_data[8] = convert_tex_coords(1.0f, 4.0f);

  *v_data[9] = (vec3){side, -side, -side};
  *t_data[9] = convert_tex_coords(0.0f, 4.0f);

  *v_data[10] = (vec3){-side, -side, -side};
  *t_data[10] = convert_tex_coords(2.0f, 1.0f);

  *v_data[11] = (vec3){-side, -side, side};
  *t_data[11] = convert_tex_coords(2.0f, 2.0f);

  *v_data[12] = (vec3){side, -side, -side};
  *t_data[12] = convert_tex_coords(-1.0f, 1.0f);

  *v_data[13] = (vec3){side, -side, side};
  *t_data[13] = convert_tex_coords(-1.0f, 2.0f);

  vector *vertices = vector_create(14);

  for (size_t i = 0; i < 14; i++) {
    vertex *new_vert = malloc(sizeof(vertex));
    *new_vert = vcf(*v_data[i], t_data[i]);
    vector_push_back(vertices, new_vert);
  }

  vector *indices = vector_create(36);
  for (size_t i = 0; i < 36; i++) {
    int *ind = malloc(sizeof(int));
    *ind = t_buffer[i];
    vector_push_back(indices, ind);
  }

  return (indexed_triangle_list){
      .vertices = vertices,
      .indices = indices,
  };
}

indexed_triangle_list cube_create_plain(float size, vertex_create_func vcf) {
  const float side = size / 2.0f;

  vector *v = vector_create(8);

  for (size_t i = 0; i < 8; i++) {
    vector_push_back(v, malloc(sizeof(vec3)));
  }

  vec3 **v_data = (vec3 **)vector_get_data(v);

  *v_data[0] = (vec3){-side, -side, -side};
  *v_data[1] = (vec3){side, -side, -side};
  *v_data[2] = (vec3){-side, side, -side};
  *v_data[3] = (vec3){side, side, -side};
  *v_data[4] = (vec3){-side, -side, side};
  *v_data[5] = (vec3){side, -side, side};
  *v_data[6] = (vec3){-side, side, side};
  *v_data[7] = (vec3){side, side, side};

  vector *vertices = vector_create(8);

  for (size_t i = 0; i < 8; i++) {
    vertex *new_vert = malloc(sizeof(vertex));
    *new_vert = vcf(*v_data[i], NULL);
    vector_push_back(vertices, new_vert);
  }

  vector *indices = vector_create(36);
  for (size_t i = 0; i < 36; i++) {
    int *ind = malloc(sizeof(int));
    *ind = t_buffer_p[i];
    vector_push_back(indices, ind);
  }

  return (indexed_triangle_list){
      .vertices = vertices,
      .indices = indices,
  };
}
