#include "drawable.h"
#include "mat3.h"
#include "renderer/renderer.h"
#include "vector.h"

drawable drawable_create(vector *model, uint32_t color) {
  return (drawable){
      .model = model,
      .color = color,
      .transformation = mat3_identity(),
  };
}

void drawable_apply_transformation(drawable *dr, const mat3 *trans_in) {
  dr->transformation = mat3_mult_mat3(trans_in, &dr->transformation);
}

void drawable_render(drawable *dr, renderer *rn) {
  renderer_create_closed_polyline(rn, dr->model, dr->transformation, dr->color);
}
