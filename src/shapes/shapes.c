#include "shapes.h"
#include "vec3.h"

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
