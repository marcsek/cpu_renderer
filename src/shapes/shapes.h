#ifndef SHAPES_H
#define SHAPES_H

#include "vector.h"
#include <stdbool.h>

typedef struct {
  vector *vertices;
  int *indices;
} indexed_line_list;

typedef struct {
  vector *vertices;
  int *indices;
  bool *cull_flags;
} indexed_triangle_list;

typedef struct {
  vector *vertices;
} cube;

cube cube_create(float size);
indexed_line_list cube_get_lines(cube *c);
indexed_triangle_list cube_get_triangles(cube *c);

vector *shape_copy_verts(vector *model);

#endif
