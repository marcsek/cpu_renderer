#ifndef SHAPES_H
#define SHAPES_H

#include "vector.h"
#include <stdbool.h>

typedef struct {
  vector *vertices;
  vector *indices;
} indexed_triangle_list;

indexed_triangle_list cube_create_skinned(float size);
indexed_triangle_list cube_create_plain(float size);

indexed_triangle_list plain_create_plain(int divisions, float size);
indexed_triangle_list plain_create_skinned(int divisions, float size);

#endif
