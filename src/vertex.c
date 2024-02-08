#include "vertex.h"
#include "vec3.h"

vertex vertex_interpolate_to(const vertex *tv, const vertex *inter_to,
                             float alpha) {
  return (vertex){
      .pos = vec3_interpolate_to(&tv->pos, &inter_to->pos, alpha),
      .tc = vec2_interpolate_to(&tv->tc, &inter_to->tc, alpha),
  };
}

vertex vertex_copy(const vertex *v) {
  return (vertex){
      .pos = vec3_copy(&v->pos),
      .tc = vec2_copy(&v->tc),
  };
}

void vertex_add(vertex *v0, const vertex *v1) {
  vec3_add(&v0->pos, &v1->pos);
  vec2_add(&v0->tc, &v1->tc);
}

void vertex_sub(vertex *v0, const vertex *v1) {
  vec3_subs(&v0->pos, &v1->pos);
  vec2_subs(&v0->tc, &v1->tc);
}

void vertex_mult(vertex *v0, float rhs) {
  vec3_mult_s(&v0->pos, rhs);
  vec2_mult_s(&v0->tc, rhs);
}

void vertex_div(vertex *v0, float rhs) {
  vec3_div_s(&v0->pos, rhs);
  vec2_div_s(&v0->tc, rhs);
}
