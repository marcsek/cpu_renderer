#include "../shaders/vertex.h"
#include "shapes.h"
#include "vec2.h"
#include "vec3.h"
#include "vector.h"
#include <stdlib.h>
#include <sys/_types/_size_t.h>

indexed_triangle_list plain_create_plain(int divisions, float size) {
  const int n_vertices = divisions + 1;

  vector *v = vector_create((n_vertices + 1) * (n_vertices + 1));

  {
    const float side = size / 2.0f;
    const float division_size = size / (float)divisions;
    const vec3 bottom_left = {-side, -side, 0.0f};

    for (int y = 0, i = 0; y < n_vertices; y++) {
      const float y_pos = ((float)y) * division_size;
      for (int x = 0; x < n_vertices; x++, i++) {
        vertex *bottom_left_c = malloc(sizeof(vertex));
        *bottom_left_c = vertex_default_create(bottom_left);
        vec3_add(&bottom_left_c->pos,
                 &(vec3){(float)x * division_size, y_pos, 0.0f});
        vector_push_back(v, bottom_left_c);
      }
    }
  }

  vector *i = vector_create((divisions * divisions) * 6);

  {
    for (size_t y = 0; y < (size_t)divisions; y++) {
      for (size_t x = 0; x < (size_t)divisions; x++) {
        size_t *idx0 = malloc(sizeof(size_t));
        size_t *idx1 = malloc(sizeof(size_t));
        size_t *idx2 = malloc(sizeof(size_t));
        size_t *idx3 = malloc(sizeof(size_t));
        *idx0 = y * n_vertices + x;
        *idx1 = y * n_vertices + x + 1;
        *idx2 = (y + 1) * n_vertices + x;
        *idx3 = (y + 1) * n_vertices + x + 1;

        vector_push_back(i, idx0);
        vector_push_back(i, idx2);
        vector_push_back(i, idx1);
        vector_push_back(i, idx1);
        vector_push_back(i, idx2);
        vector_push_back(i, idx3);
      }
    }
  }
  return (indexed_triangle_list){
      .vertices = v,
      .indices = i,
  };
}

indexed_triangle_list plain_create_skinned(int divisions, float size) {
  indexed_triangle_list it_list = plain_create_plain(divisions, size);
  vertex **vert_d = (vertex **)vector_get_data(it_list.vertices);

  {
    const int n_vertices = divisions + 1;
    const float t_divisions = 1.0f / (float)divisions;
    const vec2 t_bottom_left = {0.0f, 1.0f};

    for (int y = 0, i = 0; y < n_vertices; y++) {
      const float y_t = -(float)y * t_divisions;
      for (int x = 0; x < n_vertices; x++, i++) {
        vec2 *v2 = malloc(sizeof(vec2));
        *v2 = (vec2){
            .x = (float)x * t_divisions,
            .y = y_t,
        };
        vec2_add(v2, &t_bottom_left);
        *vert_d[i] = tex_vertex_create(vert_d[i]->pos, v2);
      }
    }
  }

  return it_list;
}
