#include "surface.h"
#include "../vendors/stb/stb_image.h"
#include "helpers/debug_info.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const int forced_pitch = 4;

static void convert_to_correct_format(uint32_t *conv, const unsigned char *old,
                                      size_t size) {
  for (size_t i = 0; i < size * forced_pitch; i += forced_pitch) {
    conv[i / forced_pitch] = 0xFF << 24 | (uint32_t)old[i] << 16 |
                             (uint32_t)old[i + 1] << 8 | (uint32_t)old[i + 2];
  }
}

surface surface_from_file(const char *filename) {
  int x = 0;
  int y = 0;
  int pitch = 0;

  unsigned char *data = stbi_load(filename, &x, &y, &pitch, forced_pitch);

  const int img_size = x * y;

  uint32_t *converted = malloc(sizeof(unsigned char) * img_size * forced_pitch);
  convert_to_correct_format(converted, data, img_size);

  if (data == NULL) {
    debug_printf("failed to load image asset \"%s\" (%s)", filename,
                 stbi_failure_reason());
    return (surface){};
  }

  debug_printf("loaded image asset -> name: \"%s\"; size: %ux%upx (%u bytes);",
               filename, x, y, 4 * img_size);

  stbi_image_free(data);
  return (surface){.p_buffer = converted, .width = x, .height = y};
}

uint32_t surface_get_pixel(surface *s, unsigned int x, unsigned int y) {
  assert(x >= 0 && x < s->width);
  assert(y >= 0 && y < s->height);
  return s->p_buffer[s->height * y + x];
}
