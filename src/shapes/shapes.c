#include "shapes.h"
#include "../texture_vertex.h"
#include "vec2.h"
#include "vec3.h"
#include <stdlib.h>

vector *shape_copy_verts(vector *model) {
  vector *new = vector_create_cf(vector_get_size(model), free);
  vec3 **data = (vec3 **)vector_get_data(model);

  for (size_t i = 0; i < vector_get_size(model); i++) {
    vec3 *v = malloc(sizeof(vec3));
    *v = vec3_copy(data[i]);
    vector_push_back(new, v);
  }
  return new;
}

vector *shape_copy_verts_tex(vector *model) {
  vector *new = vector_create_cf(vector_get_size(model), free);
  tex_vertex **data = (tex_vertex **)vector_get_data(model);

  for (size_t i = 0; i < vector_get_size(model); i++) {
    tex_vertex *tx = malloc(sizeof(tex_vertex));
    *tx = (tex_vertex){
        .pos = vec3_copy(&data[i]->pos),
        .tc = vec2_copy(&data[i]->tc),
    };
    vector_push_back(new, tx);
  }
  return new;
}
