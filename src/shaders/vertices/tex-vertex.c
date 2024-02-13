#include "../vertex.h"

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .tc = ((type *)(var)->sd) }

static vertex copy(const vertex *v) {
  tex_vertex vm = get_sd(v, vec2);
  vec2 *new_v = malloc(sizeof(vec2));
  *new_v = *vm.tc;
  return tex_vertex_create(vm.pos, new_v);
}

static vertex interpolate_to(const vertex *tv, const vertex *inter_to,
                             float alpha) {
  vertex tvc = copy(tv);
  tex_vertex tvm = get_sd(&tvc, vec2);
  tex_vertex itm = get_sd(inter_to, vec2);

  vec3 new_pos = vec3_interpolate_to(&tv->pos, &inter_to->pos, alpha);
  vec2 new_tc = vec2_interpolate_to(tvm.tc, itm.tc, alpha);

  tvm.pos = new_pos;
  *tvm.tc = new_tc;

  return tex_vertex_create(tvm.pos, tvm.tc);
}

static void add(vertex *v0, const vertex *v1) {
  tex_vertex v0m = get_sd(v0, vec2);
  tex_vertex v1m = get_sd(v1, vec2);
  vec3_add(&v0->pos, &v1->pos);
  vec2_add(v0m.tc, v1m.tc);
}

static void sub(vertex *v0, const vertex *v1) {
  tex_vertex v0m = get_sd(v0, vec2);
  tex_vertex v1m = get_sd(v1, vec2);
  vec3_subs(&v0->pos, &v1->pos);
  vec2_subs(v0m.tc, v1m.tc);
}

static void mult(vertex *v0, float rhs) {
  tex_vertex v0m = get_sd(v0, vec2);
  vec3_mult_s(&v0->pos, rhs);
  vec2_mult_s(v0m.tc, rhs);
}

static void divd(vertex *v0, float rhs) {
  tex_vertex v0m = get_sd(v0, vec2);
  vec3_div_s(&v0->pos, rhs);
  vec2_div_s(v0m.tc, rhs);
}

static void vfree(vertex *v) {
  tex_vertex vm = get_sd(v, vec2);
  free(vm.tc);
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

vertex tex_vertex_create(vec3 pos, void *sd) {
  return (vertex){
      .pos = pos,
      .sd = sd,
      .fn = &fns,
  };
}
