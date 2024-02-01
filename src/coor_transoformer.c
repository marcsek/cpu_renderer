#include "coor_transformer.h"
#include "renderer/renderer.h"
#include "shapes/shapes.h"
#include "vec.h"
#include <stdlib.h>

void coor_transformer_create_closed_polyline(coordinate_transformer *ct,
                                             vec *verts, uint32_t v) {
  vec2 offset = {(float)ct->rn->buffer_width / 2,
                 (float)ct->rn->buffer_height / 2};

  vec *new = vec_create(vec_get_size(verts));
  vec2 **data = (vec2 **)vec_get_data(verts);

  for (size_t i = 0; i < vec_get_size(verts); i++) {
    vec2 *v = malloc(sizeof(vec2));
    vec2 original = *data[i];
    v->x = original.x * -1.0f + offset.x;
    v->y = original.y * -1.0f + offset.y;
    vec_push_back(new, v);
  }

  renderer_create_closed_polyline(ct->rn, new, v);
}
