#ifndef TEXTURE_VERTEX_H
#define TEXTURE_VERTEX_H

#include <notstd.h>

typedef struct {
  struct vertex (*copy)(const struct vertex *v);
  void (*add)(struct vertex *v0, const struct vertex *v1);
  void (*sub)(struct vertex *v0, const struct vertex *v1);
  void (*mult)(struct vertex *v0, float rhs);
  void (*div)(struct vertex *v0, float rhs);
  struct vertex (*interpolate_to)(const struct vertex *tv,
                                  const struct vertex *inter_to, float alpha);
  void (*free)(struct vertex *v);
} vertex_funcs;

typedef struct vertex {
  vec3 pos;
  void *sd;
  vertex_funcs *fn;
} vertex;

struct vertex vertex_copy(const struct vertex *v);
void vertex_add(struct vertex *v0, const struct vertex *v1);
void vertex_sub(struct vertex *v0, const struct vertex *v1);
void vertex_mult(struct vertex *v0, float rhs);
void vertex_div(struct vertex *v0, float rhs);
struct vertex vertex_interpolate_to(const struct vertex *tv,
                                    const struct vertex *inter_to, float alpha);
void vertex_free(struct vertex *v);

// Custom * * * *

typedef struct {
  vec3 pos;
  vec3 *color;
} color_vertex;

typedef struct {
  vec3 pos;
  vec2 *tc;
} tex_vertex;

typedef struct {
  vec3 pos;
  uint32_t *color;
} scolor_vertex;

typedef struct {
  vec3 pos;
  vec3 *normal;
} normal_vertex;

// Contructors for each vertex shader effect
vertex vertex_default_create(vec3 pos, void *sd);
vertex tex_vertex_create(vec3 pos, void *sd);
vertex color_vertex_create(vec3 pos, void *sd);
vertex scolor_vertex_create(vec3 pos, void *sd);
vertex normal_vertex_create(vec3 pos, void *sd);

#endif
