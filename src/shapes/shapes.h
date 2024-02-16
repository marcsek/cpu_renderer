#ifndef SHAPES_H
#define SHAPES_H

#include "../indexed_triangle_list.h"
#include <stdbool.h>

indexed_triangle_list cube_create_skinned(float size, vertex_create_func vcf);
indexed_triangle_list cube_create_plain(float size, vertex_create_func vcf);

indexed_triangle_list plain_create_plain(int divisions, float size);
indexed_triangle_list plain_create_skinned(int divisions, float size);

indexed_triangle_list sphere_get_plain(float radius, int lat_div, int long_div,
                                       vertex_create_func vcf);

#endif
