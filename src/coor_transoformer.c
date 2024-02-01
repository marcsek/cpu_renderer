#include "coor_transformer.h"
#include "renderer/renderer.h"

coordinate_transformer coor_tranformer_create(renderer *rn) {
  return (coordinate_transformer){rn};
}

void coor_transformer_create_closed_polyline(coordinate_transformer *ct,
                                             vector *verts, uint32_t v) {
  vec2 offset = {(float)ct->rn->buffer_width / 2,
                 (float)ct->rn->buffer_height / 2};

  vec2 **data = (vec2 **)vector_get_data(verts);

  for (size_t i = 0; i < vector_get_size(verts); i++) {
    vec2 *v = data[i];
    v->x = v->x + offset.x;
    v->y = v->y * -1.0f + offset.y;
  }

  renderer_create_closed_polyline(ct->rn, verts, v);
}
