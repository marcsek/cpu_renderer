#include "../vertex.h"

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .color = ((type *)(var)->sd) }

static vertex copy(const vertex *v) {
  scolor_vertex vm = get_sd(v, uint32_t);
  uint32_t *new_v = malloc(sizeof(uint32_t));
  *new_v = *vm.color;
  return scolor_vertex_create(vm.pos, new_v);
}

static vertex interpolate_to(const vertex *tv, const vertex *inter_to,
                             float alpha) {
  vertex tvc = copy(tv);
  scolor_vertex tvm = get_sd(&tvc, uint32_t);

  vec3 new_pos = vec3_interpolate_to(&tv->pos, &inter_to->pos, alpha);

  tvm.pos = new_pos;

  return scolor_vertex_create(tvm.pos, tvc.sd);
}

static void add(vertex *v0, const vertex *v1) { vec3_add(&v0->pos, &v1->pos); }

static void sub(vertex *v0, const vertex *v1) { vec3_subs(&v0->pos, &v1->pos); }

static void mult(vertex *v0, float rhs) { vec3_mult_s(&v0->pos, rhs); }

static void divd(vertex *v0, float rhs) { vec3_div_s(&v0->pos, rhs); }

static void vfree(vertex *v) {
  scolor_vertex vm = get_sd(v, uint32_t);
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

vertex scolor_vertex_create(vec3 pos, void *sd) {
  return (vertex){
      .pos = pos,
      .sd = sd,
      .fn = &fns,
  };
}
