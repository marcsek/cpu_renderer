#include "../effect.h"
#include "essentials.h"
#include <stdint.h>

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .color = ((type *)(var)->sd) }

typedef struct {
  vec3 pos;
  uint32_t *color;
} mvertex;

static vertex copy(const vertex *v) {
  mvertex vm = get_sd(v, uint32_t);
  uint32_t *new_v = malloc(sizeof(uint32_t));
  *new_v = *vm.color;
  return solid_vertex_create(vm.pos, new_v);
}

static vertex interpolate_to(const vertex *tv, const vertex *inter_to,
                             float alpha) {
  vertex tvc = copy(tv);
  mvertex tvm = get_sd(&tvc, uint32_t);

  vec3 new_pos = vec3_interpolate_to(&tv->pos, &inter_to->pos, alpha);

  tvm.pos = new_pos;

  return solid_vertex_create(tvm.pos, tvc.sd);
}

static void add(vertex *v0, const vertex *v1) { vec3_add(&v0->pos, &v1->pos); }

static void sub(vertex *v0, const vertex *v1) { vec3_subs(&v0->pos, &v1->pos); }

static void mult(vertex *v0, float rhs) { vec3_mult_s(&v0->pos, rhs); }

static void divd(vertex *v0, float rhs) { vec3_div_s(&v0->pos, rhs); }

static void vfree(vertex *v) {
  mvertex vm = get_sd(v, uint32_t);
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

vertex solid_vertex_create(vec3 pos, void *sd) {
  return (vertex){
      .pos = pos,
      .sd = sd,
      .fn = &fns,
  };
}

typedef struct {
  uint32_t *colors;
} sface_shader_data;

static triangle process(void *data, const vertex *in1, const vertex *in2,
                        const vertex *in3, unsigned int index) {
  UNUSED(index);

  sface_shader_data *sd = (sface_shader_data *)data;

  uint32_t *c0 = malloc(sizeof(uint32_t));
  uint32_t *c1 = malloc(sizeof(uint32_t));
  uint32_t *c2 = malloc(sizeof(uint32_t));
  *c0 = sd->colors[index / 2];
  *c1 = sd->colors[index / 2];
  *c2 = sd->colors[index / 2];
  // printf("%d\n", index / 2);

  vertex v0_c = solid_vertex_create(in1->pos, c0);
  vertex v1_c = solid_vertex_create(in2->pos, c1);
  vertex v2_c = solid_vertex_create(in3->pos, c2);

  return (triangle){.v0 = v0_c, .v1 = v1_c, .v2 = v2_c};
}

void sface_geo_bind_colors(void *data, uint32_t *colors) {
  sface_shader_data *sd = (sface_shader_data *)data;

  sd->colors = colors;
}

geometry_shader sface_geometry_create() {
  sface_shader_data *d = malloc(sizeof(sface_shader_data));

  return (geometry_shader){
      .process = process,
      .shader_data = d,
  };
}
