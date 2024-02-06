#include "screen_transformer.h"
#include "vec3.h"

screen_transformer st_create(float x_size, float y_size) {
  return (screen_transformer){.x_size = x_size / 2.0f, .y_size = y_size / 2.0f};
}

void st_transform(screen_transformer *st, vec3 *v) {
  const float z_inv = 1.0f / v->z;
  v->x = (v->x * z_inv + 1.0f) * st->x_size;
  v->y = (-v->y * z_inv + 1.0f) * st->y_size;
}

vec3 st_get_transformed(screen_transformer *st, const vec3 *v) {
  vec3 v_c = vec3_copy(v);
  st_transform(st, &v_c);
  return v_c;
}
