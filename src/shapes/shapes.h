#ifndef SHAPES_H
#define SHAPES_H

#include <notstd.h>

typedef struct {
  float x;
  float y;
} vec2;

vec *shapes_star_make(float outerRadius, float innerRadius, int nFlares);

#endif
