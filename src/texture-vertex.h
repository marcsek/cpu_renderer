#ifndef TEXTURE_VERTEX_H
#define TEXTURE_VERTEX_H

#include <notstd.h>

typedef struct {
  vec3 pos;
  vec2 tc;
} tex_vertex;

tex_vertex tex_ver_interpolate_to(const tex_vertex *tv,
                                  const tex_vertex *inter_to, float alpha);

tex_vertex tex_ver_copy(const tex_vertex *v);
void tex_ver_add(tex_vertex *v0, const tex_vertex *v1);
void tex_ver_sub(tex_vertex *v0, const tex_vertex *v1);
void tex_ver_mult(tex_vertex *v0, float rhs);
void tex_ver_div(tex_vertex *v0, float rhs);

#endif
