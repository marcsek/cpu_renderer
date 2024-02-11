#include "screen_transformer.h"
#include "vec3.h"

screen_transformer st_create(float x_size, float y_size) {
  return (screen_transformer){.x_size = x_size / 2.0f, .y_size = y_size / 2.0f};
}

void st_transform(screen_transformer *st, vertex *v) {
  const float z_inv = 1.0f / v->pos.z;

  v->mult(v, z_inv);

  v->pos.x = (v->pos.x + 1.0f) * st->x_size;
  v->pos.y = (-v->pos.y + 1.0f) * st->y_size;

  v->pos.z = z_inv;
}

vertex st_get_transformed(screen_transformer *st, const vertex *v) {
  vertex v_c = v->copy(v);
  st_transform(st, &v_c);
  return v_c;
}
