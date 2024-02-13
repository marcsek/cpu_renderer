#ifndef ITD_H
#define ITD_H

#include "shaders/vertex.h"
#include <notstd.h>

typedef vertex (*vertex_create_func)(vec3 pos, void *sd);

typedef struct {
  vector *vertices;
  vector *indices;
} indexed_triangle_list;

indexed_triangle_list itd_load(const char *file_name, vertex_create_func vcf);
indexed_triangle_list itd_load_normals(const char *file_name,
                                       vertex_create_func vcf);

#endif
