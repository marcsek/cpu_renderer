#include "coor_transformer.h"
#include "drawable.h"
#include "renderer/renderer.h"

coordinate_transformer coor_tranformer_create(renderer *rn) {
  return (coordinate_transformer){rn};
}

void coor_transformer_draw(coordinate_transformer *ct, drawable *dr) {
  const vec2 offset = {(float)ct->rn->buffer_width / 2,
                       (float)ct->rn->buffer_height / 2};

  drawable_scale_sep(dr, &(vec2){1.0f, -1.0f});
  drawable_translate(dr, &offset);
  drawable_render(dr, ct->rn);
}
