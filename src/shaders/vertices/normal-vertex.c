#include "../vertex.h"

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .normal = ((type *)(var)->sd) }

static vertex copy(const vertex *v) {
  normal_vertex vm = get_sd(v, vec4);
  vec4 *new_v = malloc(sizeof(vec4));
  *new_v = *vm.normal;
  return scolor_vertex_create(vm.pos, new_v);
}

static vertex interpolate_to(const vertex *tv, const vertex *inter_to,
                             float alpha) {
  vertex tvc = copy(tv);
  normal_vertex tvm = get_sd(&tvc, vec4);

  vec4 new_pos = vec4_interpolate_to(&tv->pos, &inter_to->pos, alpha);

  tvm.pos = new_pos;

  return scolor_vertex_create(tvm.pos, tvc.sd);
}

static void add(vertex *v0, const vertex *v1) { vec4_add(&v0->pos, &v1->pos); }

static void sub(vertex *v0, const vertex *v1) { vec4_subs(&v0->pos, &v1->pos); }

static void mult(vertex *v0, float rhs) { vec4_mult_s(&v0->pos, rhs); }

static void divd(vertex *v0, float rhs) { vec4_div_s(&v0->pos, rhs); }

static void vfree(vertex *v) {
  normal_vertex vm = get_sd(v, vec4);
  free(vm.normal);
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

vertex normal_vertex_create(vec4 pos, void *sd) {
  return (vertex){
      .pos = pos,
      .sd = sd,
      .fn = &fns,
  };
}
