#include "../effect.h"
#include "../vertex.h"
#include "MiniFB.h"
#include "essentials.h"
#include "vec3.h"
#include <stdio.h>
#include <stdlib.h>

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .color = ((type *)(var)->sd) }

typedef struct {
  vec3 pos;
  vec3 *color;
} mvertex;

typedef struct pixel_shader_data {
} color_shader_data;

static vertex copy(const vertex *v) {
  mvertex vm = get_sd(v, vec3);
  if (v->sd == NULL) {
    return color_vertex_create(vm.pos, NULL);
  }
  vec3 *new_v = malloc(sizeof(vec3));
  *new_v = *vm.color;
  return color_vertex_create(vm.pos, new_v);
}

static vertex interpolate_to(const vertex *tv, const vertex *inter_to,
                             float alpha) {
  vertex tvc = copy(tv);
  mvertex tvm = get_sd(&tvc, vec3);
  mvertex itm = get_sd(inter_to, vec3);

  vec3 new_pos = vec3_interpolate_to(&tv->pos, &inter_to->pos, alpha);
  vec3 new_tc = vec3_interpolate_to(tvm.color, itm.color, alpha);

  tvm.pos = new_pos;
  *tvm.color = new_tc;

  return color_vertex_create(tvm.pos, tvm.color);
}

static void add(vertex *v0, const vertex *v1) {
  mvertex v0m = get_sd(v0, vec3);
  mvertex v1m = get_sd(v1, vec3);
  vec3_add(&v0->pos, &v1->pos);
  vec3_add(v0m.color, v1m.color);
}

static void sub(vertex *v0, const vertex *v1) {
  mvertex v0m = get_sd(v0, vec3);
  mvertex v1m = get_sd(v1, vec3);
  vec3_subs(&v0->pos, &v1->pos);
  vec3_subs(v0m.color, v1m.color);
}

static void mult(vertex *v0, float rhs) {
  mvertex v0m = get_sd(v0, vec3);
  vec3_mult_s(&v0->pos, rhs);
  vec3_mult_s(v0m.color, rhs);
}

static void divd(vertex *v0, float rhs) {
  mvertex v0m = get_sd(v0, vec3);
  vec3_div_s(&v0->pos, rhs);
  vec3_div_s(v0m.color, rhs);
}

static void vfree(vertex *v) {
  mvertex vm = get_sd(v, vec3);
  free(vm.color);
}

static vertex_funcs fns = (vertex_funcs){
    .interpolate_to = interpolate_to,
    .copy = copy,
    .add = add,
    .sub = sub,
    .mult = mult,
    .div = divd,
    .free = vfree,
};

vertex color_vertex_create(vec3 pos, void *sd) {
  return (vertex){
      .pos = pos,
      .sd = sd,
      .fn = &fns,
  };
}

static uint32_t create_pixel(void *data, const vertex *inv) {
  UNUSED(data);
  mvertex in = get_sd(inv, vec3);

  return MFB_ARGB(0xFF, in.color->x, in.color->y, in.color->z);
}

pixel_shader color_shader_create() {
  return (pixel_shader){
      .create_pixel = create_pixel,
  };
}
