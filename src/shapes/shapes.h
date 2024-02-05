#ifndef SHAPES_H
#define SHAPES_H

#include "vector.h"
#include <stdlib.h>

typedef struct {
  vector *vertices;
  int *indices;
} indexed_line_list;

typedef struct {
  vector *vertices;
} cube;

cube cube_create(float size);
indexed_line_list cube_get_lines(cube *c);

#endif
