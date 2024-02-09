#ifndef SHAPES_H
#define SHAPES_H

#include "vector.h"
#include <stdbool.h>

typedef struct {
  vector *vertices;
  int *indices;
} indexed_triangle_list;

typedef struct shape {
  indexed_triangle_list (*get_skinned)(struct shape *sh);
  vector *vertices;
} shape;

shape cube_create(float size);
shape cube_folded_create(float size);

#endif
