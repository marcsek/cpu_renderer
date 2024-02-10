#ifndef SHAPES_H
#define SHAPES_H

#include "vector.h"
#include <stdbool.h>

typedef struct {
  vector *vertices;
  int *indices;
} indexed_triangle_list;

indexed_triangle_list cube_create_skinned(float size);
indexed_triangle_list cube_create_plain(float size);

#endif
