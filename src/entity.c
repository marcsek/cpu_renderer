#include "entity.h"
#include "drawable.h"
#include "vec2.h"

vector *copy_model(entity *en);

entity entity_create(vector *model, uint32_t color) {
  return (entity){
      .pos = {0.0f, 0.0f},
      .model = model,
      .scale = 1.0f,
      .color = color,
      .angle = 0.0f,
  };
}

void entity_translate_by(entity *en, vec2 v) {
  en->pos.x += v.x;
  en->pos.y += v.y;
}

void entity_rotate_by(entity *en, float angle_in) { en->angle += angle_in; }

drawable entity_get_drawable(entity *en) {
  drawable dr = drawable_create(copy_model(en), en->color);
  mat3 rot = mat3_rotation(en->angle);
  mat3 scale = mat3_scale(en->scale);
  mat3 transl = mat3_translation(&en->pos);
  mat3 intermediate = mat3_mult_mat3(&transl, &scale);
  mat3 transform = mat3_mult_mat3(&intermediate, &rot);
  drawable_apply_transformation(&dr, &transform);
  return dr;
}

vector *copy_model(entity *en) {
  vector *new = vector_create_cf(vector_get_size(en->model), free);
  vec2 **data = (vec2 **)vector_get_data(en->model);

  for (size_t i = 0; i < vector_get_size(en->model); i++) {
    vec2 *v = malloc(sizeof(vec2));
    vec2 original = *data[i];
    v->x = original.x;
    v->y = original.y;
    vector_push_back(new, v);
  }
  return new;
}
