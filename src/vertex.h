#ifndef TEXTURE_VERTEX_H
#define TEXTURE_VERTEX_H

#include <notstd.h>

typedef struct {
  vec3 pos;
  vec2 tc;
} vertex;

vertex vertex_interpolate_to(const vertex *tv, const vertex *inter_to,
                             float alpha);

vertex vertex_copy(const vertex *v);
void vertex_add(vertex *v0, const vertex *v1);
void vertex_sub(vertex *v0, const vertex *v1);
void vertex_mult(vertex *v0, float rhs);
void vertex_div(vertex *v0, float rhs);

#endif
