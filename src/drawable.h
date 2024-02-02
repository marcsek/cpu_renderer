#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "renderer/renderer.h"
#include <notstd.h>

typedef struct {
  vector *model;
  vec2 translation;
  vec2 scale;
  uint32_t color;
} drawable;

drawable drawable_create(vector *model, uint32_t color);
void drawable_translate(drawable *dr, const vec2 *translation_in);
void drawable_scale(drawable *dr, float scale_in);
void drawable_scale_sep(drawable *dr, const vec2 *scale_in);
void drawable_render(drawable *dr, renderer *rn);

#endif
