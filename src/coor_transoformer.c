#include "coor_transformer.h"
#include "drawable.h"
#include "mat3.h"
#include "renderer/renderer.h"

coordinate_transformer coor_tranformer_create(renderer *rn) {
  return (coordinate_transformer){rn};
}

void coor_transformer_draw(coordinate_transformer *ct, drawable *dr) {
  const vec2 offset = {(float)ct->rn->buffer_width / 2,
                       (float)ct->rn->buffer_height / 2};

  mat3 scale = mat3_scale_ind(1.0f, -1.0f);
  mat3 trans = mat3_translation(&offset);
  mat3 transform = mat3_mult_mat3(&trans, &scale);
  drawable_apply_transformation(dr, &transform);

  drawable_render(dr, ct->rn);
}
