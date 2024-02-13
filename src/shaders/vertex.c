#include "vertex.h"

struct vertex vertex_copy(const struct vertex *v) { return v->fn->copy(v); }

void vertex_add(struct vertex *v0, const struct vertex *v1) {
  v0->fn->add(v0, v1);
}

void vertex_sub(struct vertex *v0, const struct vertex *v1) {
  v0->fn->sub(v0, v1);
}

void vertex_mult(struct vertex *v0, float rhs) { v0->fn->mult(v0, rhs); }

void vertex_div(struct vertex *v0, float rhs) { v0->fn->div(v0, rhs); }

struct vertex vertex_interpolate_to(const struct vertex *tv,
                                    const struct vertex *inter_to,
                                    float alpha) {
  return tv->fn->interpolate_to(tv, inter_to, alpha);
}

void vertex_free(struct vertex *v) { v->fn->free(v); }
