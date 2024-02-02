#ifndef ENTITY_H
#define ENTITY_H

#include "drawable.h"
#include <notstd.h>
#include <stdint.h>

typedef struct {
  vec2 pos;
  vector *model;
  float scale;
  uint32_t color;
} entity;

entity entity_create(vector *model, uint32_t color);
void entity_translate_by(entity *en, vec2 v);
drawable entity_get_drawable(entity *en);

#endif
