#ifndef TEXTURE_VERTEX_H
#define TEXTURE_VERTEX_H

#include <notstd.h>

typedef struct vertex {
  vec3 pos;
  void *sd;
  struct vertex (*copy)(const struct vertex *v);
  void (*add)(struct vertex *v0, const struct vertex *v1);
  void (*sub)(struct vertex *v0, const struct vertex *v1);
  void (*mult)(struct vertex *v0, float rhs);
  void (*div)(struct vertex *v0, float rhs);
  struct vertex (*interpolate_to)(const struct vertex *tv,
                                  const struct vertex *inter_to, float alpha);
  void (*free)(struct vertex *v);
} vertex;

vertex tex_vertex_create(vec3 pos, void *sd);
vertex color_vertex_create(vec3 pos, void *sd);

#endif
