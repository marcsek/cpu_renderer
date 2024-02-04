#include "drawable.h"
#include "renderer/renderer.h"
#include "vec2.h"
#include "vector.h"

drawable drawable_create(vector *model, uint32_t color) {
  return (drawable){
      .model = model,
      .translation = (vec2){0.0f, 0.0f},
      .color = color,
      .scale = (vec2){1.0f, 1.0f},
      .angle = 0.0f,
  };
}

void drawable_translate(drawable *dr, const vec2 *translation_in) {
  vec2_add(&dr->translation, translation_in);
}

void drawable_scale(drawable *dr, float scale_in) {
  vec2_mult(&dr->scale, &(vec2){scale_in, scale_in});
  vec2_mult(&dr->translation, &(vec2){scale_in, scale_in});
}

void drawable_scale_sep(drawable *dr, const vec2 *scale_in) {
  vec2_mult(&dr->scale, scale_in);
  vec2_mult(&dr->translation, scale_in);
}

void drawable_rotate(drawable *dr, float angle_in) {
  dr->angle += angle_in;
  vec2_rotate(&dr->translation, angle_in);
}

void drawable_render(drawable *dr, renderer *rn) {
  vec2 **data = (vec2 **)vector_get_data(dr->model);
  for (size_t i = 0; i < vector_get_size(dr->model); i++) {
    vec2 *v = data[i];
    vec2_rotate(v, dr->angle);
    vec2_mult(v, &dr->scale);
    vec2_add(v, &dr->translation);
  }
  renderer_create_closed_polyline(rn, dr->model, dr->color);
}
