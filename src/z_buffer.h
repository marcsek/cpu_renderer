#ifndef Z_BUFFER_H
#define Z_BUFFER_H

#include <stdbool.h>

typedef struct {
  int width;
  int height;
  float *p_buffer;
} z_buffer;

z_buffer z_buffer_create(int width, int height);
void z_buffer_destroy(z_buffer *z);
void z_buffer_clear(z_buffer *z);
bool z_buffer_test_and_set(z_buffer *z, int x, int y, float depth);

#endif
