#include "renderer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t *base_buffer = NULL;

struct _renderer {
  size_t buffer_width;
  size_t buffer_height;
  size_t buffer_size;
  uint32_t *buffer;
};

renderer *renderer_create(size_t buffer_width, size_t buffer_height) {
  size_t buffer_length = buffer_width * buffer_height;

  uint32_t *buffer = (uint32_t *)calloc(buffer_length, sizeof(uint32_t));
  base_buffer = (uint32_t *)calloc(buffer_length, sizeof(uint32_t));

  renderer *rn = malloc(sizeof(renderer));
  rn->buffer = buffer;
  rn->buffer_width = buffer_width;
  rn->buffer_height = buffer_height;
  rn->buffer_size = buffer_length * sizeof(uint32_t);
  return rn;
}

uint32_t *renderer_get_buffer(renderer *rn) { return rn->buffer; }

void renderer_put_pixel(renderer *rn, size_t x, size_t y, uint32_t v) {
  rn->buffer[rn->buffer_width * y + x] = v;
}

void renderer_reset_buffer(renderer *rn) {
  memcpy(rn->buffer, base_buffer, rn->buffer_size);
}

uint32_t *renderer_destroy(renderer *rn) {
  uint32_t *buffer = rn->buffer;

  free(base_buffer);
  base_buffer = NULL;
  free(rn);

  return buffer;
}
