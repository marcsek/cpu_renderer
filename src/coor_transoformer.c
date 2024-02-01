#include "coor_transformer.h"
#include "renderer/renderer.h"
#include "shapes/shapes.h"
#include "vec.h"

coordinate_transformer coor_tranformer_create(renderer *rn) {
  return (coordinate_transformer){rn};
}

void coor_transformer_create_closed_polyline(coordinate_transformer *ct,
                                             vec *verts, uint32_t v) {
  vec2 offset = {(float)ct->rn->buffer_width / 2,
                 (float)ct->rn->buffer_height / 2};

  vec2 **data = (vec2 **)vec_get_data(verts);

  for (size_t i = 0; i < vec_get_size(verts); i++) {
    vec2 *v = data[i];
    v->x = v->x + offset.x;
    v->y = v->y * -1.0f + offset.y;
  }

  renderer_create_closed_polyline(ct->rn, verts, v);
}
