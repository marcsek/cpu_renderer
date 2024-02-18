#include "screen_transformer.h"

screen_transformer st_create(float x_size, float y_size) {
  return (screen_transformer){.x_size = x_size / 2.0f, .y_size = y_size / 2.0f};
}

void st_transform(screen_transformer *st, vertex *v) {
  const float w_inv = 1.0f / v->pos.w;

  vertex_mult(v, w_inv);

  v->pos.x = (v->pos.x + 1.0f) * st->x_size;
  v->pos.y = (-v->pos.y + 1.0f) * st->y_size;

  v->pos.w = w_inv;
}

vertex st_get_transformed(screen_transformer *st, const vertex *v) {
  vertex v_c = vertex_copy(v);
  st_transform(st, &v_c);
  return v_c;
}
