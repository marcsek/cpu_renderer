#include "../effect.h"
#include "vec3.h"
#include <math.h>
#include <stdlib.h>

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .color = ((type *)(var)->sd) }

typedef struct {
  mat3 rotation;
  vec3 translation;
} default_shader_data;

typedef struct {
  vec3 pos;
  vec3 *color;
} mvertex;

static vertex copy(const vertex *v) {
  mvertex vm = get_sd(v, vec3);
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

static vertex create_vertex(vec3 pos, void *sd) {
  return (vertex){
      .pos = pos,
      .sd = sd,
      .fn = &fns,
  };
}

static void bind_translation(vertex_shader *vs, const vec3 in) {
  default_shader_data *sd = (default_shader_data *)vs->shader_data;
  sd->translation = in;
}

static void bind_rotation(vertex_shader *vs, const mat3 in) {
  default_shader_data *sd = (default_shader_data *)vs->shader_data;
  sd->rotation = in;
}

static vertex transform(void *data, vertex *in) {
  default_shader_data *sd = (default_shader_data *)data;
  vec3 pos_rot = mat3_mult_vec3(&sd->rotation, &in->pos);
  vec3_add(&pos_rot, &sd->translation);

  vec3 *color = malloc(sizeof(vec3));
  *color =
      (vec3){.x = fabs(pos_rot.x), .y = fabs(pos_rot.y), .z = fabs(pos_rot.z)};
  vec3_mult_s(color, 255.0f);

  vertex new_vertex = create_vertex(pos_rot, color);

  return new_vertex;
}

vertex_shader position_vertex_create() {
  default_shader_data *d = malloc(sizeof(default_shader_data));
  return (vertex_shader){
      .bind_translation = bind_translation,
      .bind_rotation = bind_rotation,
      .transform = transform,
      .shader_data = d,
  };
}
