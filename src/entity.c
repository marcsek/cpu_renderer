#include "entity.h"

entity entity_create(vector *model) {
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

vector *entity_get_polyline(entity *en) {
  vector *new = vector_create_cf(vector_get_size(en->model), free);
  vec2 **data = (vec2 **)vector_get_data(en->model);

  for (size_t i = 0; i < vector_get_size(en->model); i++) {
    vec2 *v = malloc(sizeof(vec2));
    vec2 original = *data[i];
    v->x = original.x * en->scale + en->pos.x;
    v->y = original.y * en->scale + en->pos.y;
    vector_push_back(new, v);
  }

  return new;
}

void _vector_free_func() {}
