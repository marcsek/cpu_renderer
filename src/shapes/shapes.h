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
  int *indices;
} indexed_triangle_list_tex;

typedef struct shape {
  indexed_triangle_list_tex (*get_skinned)(struct shape *sh);
  vector *vertices;
} shape;

shape cube_create(float size);
shape cube_folded_create(float size);

vector *shape_copy_verts(vector *model);
vector *shape_copy_verts_tex(vector *model);

#endif
