#include "texture_vertex.h"
#include "vec3.h"

tex_vertex tex_ver_interpolate_to(const tex_vertex *tv,
                                  const tex_vertex *inter_to, float alpha) {
  return (tex_vertex){
      .pos = vec3_interpolate_to(&tv->pos, &inter_to->pos, alpha),
      .tc = vec2_interpolate_to(&tv->tc, &inter_to->tc, alpha),
  };
}

tex_vertex tex_ver_copy(const tex_vertex *v) {
  return (tex_vertex){
      .pos = vec3_copy(&v->pos),
      .tc = vec2_copy(&v->tc),
  };
}

void tex_ver_add(tex_vertex *v0, const tex_vertex *v1) {
  vec3_add(&v0->pos, &v1->pos);
  vec2_add(&v0->tc, &v1->tc);
}

void tex_ver_sub(tex_vertex *v0, const tex_vertex *v1) {
  vec3_subs(&v0->pos, &v1->pos);
  vec2_subs(&v0->tc, &v1->tc);
}

void tex_ver_mult(tex_vertex *v0, float rhs) {
  vec3_mult_s(&v0->pos, rhs);
  vec2_mult_s(&v0->tc, rhs);
}

void tex_ver_div(tex_vertex *v0, float rhs) {
  vec3_div_s(&v0->pos, rhs);
  vec2_div_s(&v0->tc, rhs);
}
