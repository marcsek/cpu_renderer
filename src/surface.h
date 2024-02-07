#ifndef SURFACE_H
#define SURFACE_H

#include <stdint.h>

typedef struct {
  uint32_t *p_buffer;
  unsigned int width;
  unsigned int height;
} surface;

surface surface_from_file(const char *filename);
uint32_t surface_get_pixel(surface *s, unsigned int x, unsigned int y);

#endif
