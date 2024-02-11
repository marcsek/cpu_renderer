#ifndef SCREEN_TRANSFORMER_H
#define SCREEN_TRANSFORMER_H

#include "shaders/vertex.h"

typedef struct {
  float x_size;
  float y_size;
} screen_transformer;

screen_transformer st_create(float x_size, float y_size);
void st_transform(screen_transformer *st, vertex *v);
vertex st_get_transformed(screen_transformer *st, const vertex *v);

#endif
