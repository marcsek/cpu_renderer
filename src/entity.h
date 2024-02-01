#ifndef ENTITY_H
#define ENTITY_H

#include "./shapes/shapes.h"

typedef struct {
  vec2 pos;
  vector *model;
  float scale;
} entity;

entity entity_create(vector *model);
void entity_translate_by(entity *en, vec2 v);
vector *entity_get_polyline(entity *en);
void entity_destroy_polyline(entity *en);

#endif
