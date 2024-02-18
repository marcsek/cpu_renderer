#include "clip_cull_trian.h"
#include "shaders/vertex.h"
#include "triangle.h"
#include <math.h>

static void clip_1(pipeline *p, vertex *v0, vertex *v1, vertex *v2,
                   pptv pptv_func) {
  const float alphaA = (-v0->pos.z) / (v1->pos.z - v0->pos.z);
  const float alphaB = (-v0->pos.z) / (v2->pos.z - v0->pos.z);

  const vertex v2_c = vertex_copy(v2);

  const vertex v0a = vertex_interpolate_to(v0, v1, alphaA);
  const vertex v0b = vertex_interpolate_to(v0, v2, alphaB);

  const vertex v0a_c = vertex_copy(&v0a);

  pptv_func(p, (triangle){.v0 = v0a, .v1 = *v1, .v2 = v2_c});
  pptv_func(p, (triangle){.v0 = v0b, .v1 = v0a_c, .v2 = *v2});
}

static void clip_2(pipeline *p, vertex *v0, vertex *v1, vertex *v2,
                   pptv pptv_func) {
  const float alphaA = (-v0->pos.z) / (v2->pos.z - v0->pos.z);
  const float alphaB = (-v1->pos.z) / (v2->pos.z - v1->pos.z);

  const vertex v2_c = vertex_copy(v2);

  const vertex v0_n = vertex_interpolate_to(v0, v2, alphaA);
  const vertex v1_n = vertex_interpolate_to(v1, v2, alphaB);

  pptv_func(p, (triangle){.v0 = v0_n, .v1 = v1_n, .v2 = v2_c});
}

void clip_cull_triangle(pipeline *p, triangle *t, pptv pptv_func) {
  if (fabs(t->v0.pos.x) > t->v0.pos.w && fabs(t->v1.pos.x) > t->v1.pos.w &&
      fabs(t->v2.pos.x) > t->v2.pos.w)
    return;

  if (fabs(t->v0.pos.y) > t->v0.pos.w && fabs(t->v1.pos.y) > t->v1.pos.w &&
      fabs(t->v2.pos.y) > t->v2.pos.w)
    return;

  if (t->v0.pos.z > t->v0.pos.w && t->v1.pos.z > t->v1.pos.w &&
      t->v2.pos.z > t->v2.pos.w)
    return;

  if (t->v0.pos.z < 0.0f && t->v1.pos.z < 0.0f && t->v2.pos.z < 0.0f)
    return;

  if (t->v0.pos.z < 0.0f) {
    if (t->v1.pos.z < 0.0f) {
      clip_2(p, &t->v0, &t->v1, &t->v2, pptv_func);
    } else if (t->v2.pos.z < 0.0f) {
      clip_2(p, &t->v0, &t->v2, &t->v1, pptv_func);
    } else {
      clip_1(p, &t->v0, &t->v1, &t->v2, pptv_func);
    }
  } else if (t->v1.pos.z < 0.0f) {
    if (t->v2.pos.z < 0.0f) {
      clip_2(p, &t->v1, &t->v2, &t->v0, pptv_func);
    } else {
      clip_1(p, &t->v1, &t->v0, &t->v2, pptv_func);
    }
  } else if (t->v2.pos.z < 0.0f) {
    clip_1(p, &t->v2, &t->v0, &t->v1, pptv_func);
  } else {
    pptv_func(p, *t);
  }
}
