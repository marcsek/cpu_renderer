#ifndef TEXTURE_VERTEX_H
#define TEXTURE_VERTEX_H

#include <notstd.h>

typedef struct {
  vec3 pos;
  vec2 tc;
} tex_vertex;

tex_vertex tex_ver_interpolate_to(tex_vertex *tv, const tex_vertex *inter_to,
                                  float alpha);

#endif
