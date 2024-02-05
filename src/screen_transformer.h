#ifndef SCREEN_TRANSFORMER_H
#define SCREEN_TRANSFORMER_H

#include "vec3.h"

typedef struct {
  float x_size;
  float y_size;
} screen_transformer;

screen_transformer st_create(float x_size, float y_size);
void st_transform(screen_transformer *st, vec3 *v);
vec3 st_get_transformed(screen_transformer *st, const vec3 *v);

#endif
