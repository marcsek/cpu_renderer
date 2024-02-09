#include "../../surface.h"
#include "../effect.h"
#include "../vertex.h"
#include "vec2.h"
#include "vec3.h"
#include <stdlib.h>

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .tc = ((type *)(var)->sd) }

typedef struct {
  vec3 pos;
  vec2 *tc;
} mvertex;

typedef struct pixel_shader_data {
  surface sf;
  float tex_xclamp;
  float tex_yclamp;
} texture_shader_data;

static vertex copy(const vertex *v) {
  mvertex vm = get_sd(v, vec2);
  vec2 *new_v = malloc(sizeof(vec2));
  *new_v = *vm.tc;
  return tex_vertex_create(vm.pos, new_v);
}

static vertex interpolate_to(const vertex *tv, const vertex *inter_to,
                             float alpha) {
  vertex tvc = copy(tv);
  mvertex tvm = get_sd(&tvc, vec2);
  mvertex itm = get_sd(inter_to, vec2);

  vec3 new_pos = vec3_interpolate_to(&tv->pos, &inter_to->pos, alpha);
  vec2 new_tc = vec2_interpolate_to(tvm.tc, itm.tc, alpha);

  tvm.pos = new_pos;
  *tvm.tc = new_tc;

  return tex_vertex_create(tvm.pos, tvm.tc);
}

static void add(vertex *v0, const vertex *v1) {
  mvertex v0m = get_sd(v0, vec2);
  mvertex v1m = get_sd(v1, vec2);
  vec3_add(&v0->pos, &v1->pos);
  vec2_add(v0m.tc, v1m.tc);
}

static void sub(vertex *v0, const vertex *v1) {
  mvertex v0m = get_sd(v0, vec2);
  mvertex v1m = get_sd(v1, vec2);
  vec3_subs(&v0->pos, &v1->pos);
  vec2_subs(v0m.tc, v1m.tc);
}

static void mult(vertex *v0, float rhs) {
  mvertex v0m = get_sd(v0, vec2);
  vec3_mult_s(&v0->pos, rhs);
  vec2_mult_s(v0m.tc, rhs);
}

static void divd(vertex *v0, float rhs) {
  mvertex v0m = get_sd(v0, vec2);
  vec3_div_s(&v0->pos, rhs);
  vec2_div_s(v0m.tc, rhs);
}

static void vfree(vertex *v) {
  mvertex vm = get_sd(v, vec2);
  free(vm.tc);
}

vertex tex_vertex_create(vec3 pos, void *sd) {
  return (vertex){
      .pos = pos,
      .sd = sd,
      .interpolate_to = interpolate_to,
      .copy = copy,
      .add = add,
      .sub = sub,
      .mult = mult,
      .div = divd,
      .free = vfree,
  };
}

static uint32_t create_pixel(void *data, const vertex *inv) {
  texture_shader_data *px = (texture_shader_data *)data;
  mvertex in = get_sd(inv, vec2);

  return surface_get_pixel(
      &px->sf,
      (int)MIN(in.tc->x * ((float)px->sf.width) + 0.5f, px->tex_xclamp),
      (int)MIN(in.tc->y * ((float)px->sf.height) + 0.5f, px->tex_yclamp));
}

void texture_shader_bind_texture(texture_shader_data *px,
                                 const char *file_name) {
  px->sf = surface_from_file(file_name);
  px->tex_xclamp = ((float)px->sf.width) - 1.0f;
  px->tex_yclamp = ((float)px->sf.height) - 1.0f;
}

pixel_shader texture_shader_create() {
  texture_shader_data *d = malloc(sizeof(texture_shader_data));
  return (pixel_shader){
      .create_pixel = create_pixel,
      .shader_data = d,
  };
}
