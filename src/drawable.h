#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "mat3.h"
#include "renderer/renderer.h"
#include <notstd.h>

typedef struct {
  vector *model;
  mat3 transformation;
  uint32_t color;
} drawable;

drawable drawable_create(vector *model, uint32_t color);
void drawable_apply_transformation(drawable *dr, const mat3 *trans_in);
void drawable_render(drawable *dr, renderer *rn);

#endif
