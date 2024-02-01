#include "entity.h"

entity entity_create(vec *model) {
  return (entity){
      .pos = {0.0f, 0.0f},
      .model = model,
      .scale = 1.0f,
  };
}

void entity_translate_by(entity *en, vec2 v) {
  en->pos.x += v.x;
  en->pos.y += v.y;
}

vec *entity_get_polyline(entity *en) {
  vec *new = vec_create(vec_get_size(en->model));
  vec2 **data = (vec2 **)vec_get_data(en->model);

  for (size_t i = 0; i < vec_get_size(en->model); i++) {
    vec2 *v = malloc(sizeof(vec2));
    vec2 original = *data[i];
    v->x = original.x * en->scale + en->pos.x;
    v->y = original.y * en->scale + en->pos.y;
    vec_push_back(new, v);
  }

  return new;
}
