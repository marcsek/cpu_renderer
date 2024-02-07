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

typedef struct shape {
  indexed_line_list (*get_lines)(struct shape *sh);
  indexed_triangle_list (*get_triangles)(struct shape *sh);
  vector *vertices;
} shape;

shape cube_create(float size);

vector *shape_copy_verts(vector *model);

#endif
