#include "../vertex.h"

static vertex copy(const vertex *v) {
  return vertex_default_create(v->pos, NULL);
}

static vertex interpolate_to(const vertex *tv, const vertex *inter_to,
                             float alpha) {
  vec3 new_pos = vec3_interpolate_to(&tv->pos, &inter_to->pos, alpha);

  return vertex_default_create(new_pos, NULL);
}

static void add(vertex *v0, const vertex *v1) { vec3_add(&v0->pos, &v1->pos); }

static void sub(vertex *v0, const vertex *v1) { vec3_subs(&v0->pos, &v1->pos); }

static void mult(vertex *v0, float rhs) { vec3_mult_s(&v0->pos, rhs); }

static void divd(vertex *v0, float rhs) { vec3_div_s(&v0->pos, rhs); }

static void vfree(vertex *v) { UNUSED(v); }

static vertex_funcs fns = (vertex_funcs){
    .interpolate_to = interpolate_to,
    .copy = copy,
    .add = add,
    .sub = sub,
    .mult = mult,
    .div = divd,
    .free = vfree,
};

vertex vertex_default_create(vec3 pos, void *sd) {
  UNUSED(sd);
  return (vertex){
      .pos = pos,
      .sd = NULL,
      .fn = &fns,
  };
}
