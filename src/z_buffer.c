#include "z_buffer.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static float *base_buffer = NULL;

z_buffer z_buffer_create(int width, int height) {
  float *pb = malloc(width * height * sizeof(float));
  base_buffer = malloc(width * height * sizeof(float));

  // initialize base_buffer
  for (size_t i = 0; i < (size_t)width * height; i++) {
    base_buffer[i] = INFINITY;
  }

  return (z_buffer){
      .width = width,
      .height = height,
      .p_buffer = pb,
  };
}

void z_buffer_destroy(z_buffer *z) { free(z->p_buffer); }

void z_buffer_clear(z_buffer *z) {
  memcpy(z->p_buffer, base_buffer, z->width * z->height * sizeof(float));
}

static float *at(z_buffer *z, int x, int y) {
  assert(x > 0 && x < z->width);
  assert(y > 0 && y < z->height);
  return &z->p_buffer[y * z->width + x];
}

bool z_buffer_test_and_set(z_buffer *z, int x, int y, float depth) {
  float *depth_in_buffer = at(z, x, y);
  if (depth < *depth_in_buffer) {
    *depth_in_buffer = depth;
    return true;
  }
  return false;
}
