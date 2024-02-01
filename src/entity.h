#ifndef ENTITY_H
#define ENTITY_H

#include "./shapes/shapes.h"

typedef struct {
  vec2 pos;
  vec *model;
  float scale;
} entity;

entity entity_create(vec *model);
void entity_translate_by(entity *en, vec2 v);
vec *entity_get_polyline(entity *en);

#endif
