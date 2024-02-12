#ifndef SHAPES_H
#define SHAPES_H

#include "../shaders/vertex.h"
#include "vec3.h"
#include "vector.h"
#include <stdbool.h>

typedef vertex (*vertex_create_func)(vec3 pos, void *sd);

typedef struct {
  vector *vertices;
  vector *indices;
} indexed_triangle_list;

indexed_triangle_list cube_create_skinned(float size, vertex_create_func vcf);
indexed_triangle_list cube_create_plain(float size, vertex_create_func vcf);

indexed_triangle_list plain_create_plain(int divisions, float size);
indexed_triangle_list plain_create_skinned(int divisions, float size);

#endif
