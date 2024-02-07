#include "texture-vertex.h"
#include "vec3.h"

tex_vertex tex_ver_interpolate_to(tex_vertex *tv, const tex_vertex *inter_to,
                                  float alpha) {
  return (tex_vertex){
      .pos = vec3_interpolate_to(&tv->pos, &inter_to->pos, alpha),
      .tc = vec2_interpolate_to(&tv->tc, &inter_to->tc, alpha),
  };
}
