#include "vertex.h"

struct vertex vertex_copy(const struct vertex *v) { return v->fn->copy(v); }

void vertex_add(struct vertex *v0, const struct vertex *v1) {
  v0->fn->add(v0, v1);
}

void vertex_sub(struct vertex *v0, const struct vertex *v1) {
  v0->fn->sub(v0, v1);
}

void vertex_mult(struct vertex *v0, float rhs) { v0->fn->mult(v0, rhs); }

void vertex_div(struct vertex *v0, float rhs) { v0->fn->div(v0, rhs); }

struct vertex vertex_interpolate_to(const struct vertex *tv,
                                    const struct vertex *inter_to,
                                    float alpha) {
  return tv->fn->interpolate_to(tv, inter_to, alpha);
}

void vertex_free(struct vertex *v) { v->fn->free(v); }

// Default vertex
static vertex copy(const vertex *v) { return vertex_default_create(v->pos); }

static vertex interpolate_to(const vertex *tv, const vertex *inter_to,
                             float alpha) {
  vec3 new_pos = vec3_interpolate_to(&tv->pos, &inter_to->pos, alpha);

  return vertex_default_create(new_pos);
}

static void add(vertex *v0, const vertex *v1) { vec3_add(&v0->pos, &v1->pos); }

static void sub(vertex *v0, const vertex *v1) { vec3_subs(&v0->pos, &v1->pos); }

static void mult(vertex *v0, float rhs) { vec3_mult_s(&v0->pos, rhs); }

static void divd(vertex *v0, float rhs) { vec3_div_s(&v0->pos, rhs); }

static void vfree(vertex *v) {}

static vertex_funcs fns = (vertex_funcs){
    .interpolate_to = interpolate_to,
    .copy = copy,
    .add = add,
    .sub = sub,
    .mult = mult,
    .div = divd,
    .free = vfree,
};

vertex vertex_default_create(vec3 pos) {
  return (vertex){
      .pos = pos,
      .sd = NULL,
      .fn = &fns,
  };
}
