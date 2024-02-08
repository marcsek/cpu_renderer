#include "renderer.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *base_buffer = NULL;

renderer renderer_create(size_t buffer_width, size_t buffer_height) {
  size_t buffer_length = buffer_width * buffer_height;

  uint32_t *buffer = (uint32_t *)calloc(buffer_length, sizeof(uint32_t));
  base_buffer = (uint32_t *)calloc(buffer_length, sizeof(uint32_t));

  renderer rn = (renderer){
      ._buffer = buffer,
      .buffer_width = buffer_width,
      .buffer_height = buffer_height,
      .buffer_size = buffer_length *= sizeof(uint32_t),
  };
  return rn;
}

uint32_t *renderer_get_buffer(renderer *rn) { return rn->_buffer; }

void renderer_put_pixel(renderer *rn, unsigned int x, unsigned int y,
                        uint32_t v) {
  assert(x >= 0 && x < rn->buffer_width);
  assert(y >= 0 && y < rn->buffer_height);
  rn->_buffer[rn->buffer_width * y + x] = v;
}

void renderer_reset_buffer(renderer *rn) {
  memcpy(rn->_buffer, base_buffer, rn->buffer_size);
}

uint32_t *renderer_destroy(renderer *rn) {
  uint32_t *buffer = rn->_buffer;

  free(base_buffer);
  base_buffer = NULL;
  free(rn);

  return buffer;
}

void renderer_create_line(renderer *rn, int x1, int y1, int x2, int y2,
                          uint32_t v) {
  float m = 0.0f;
  if (x1 != x2) {
    m = (float)(y2 - y1) / (x2 - x1);
  }

  if (x1 != x2 && fabs(m) <= 1.0f) {
    if (x1 > x2) {
      int tmpx = x1;
      int tmpy = y1;
      x1 = x2;
      x2 = tmpx;
      y1 = y2;
      y2 = tmpy;
    }

    float b = y1 - m * x1;

    for (int x = x1; x < x2; x++) {
      int y = (int)(m * x + b);

      if (y >= 0 && y < rn->buffer_height && x >= 0 && x < rn->buffer_width) {
        renderer_put_pixel(rn, x, y, v);
      }
    }
  } else {
    if (y1 > y2) {
      int tmpx = x1;
      int tmpy = y1;
      x1 = x2;
      x2 = tmpx;
      y1 = y2;
      y2 = tmpy;
    }

    float w = (float)(x2 - x1) / (y2 - y1);
    float p = x1 - w * y1;

    for (int y = y1; y < y2; y++) {
      int x = (int)(w * y + p);
      if (y >= 0 && y < rn->buffer_height && x >= 0 && x < rn->buffer_width) {
        renderer_put_pixel(rn, x, y, v);
      }
    }
  }
}
