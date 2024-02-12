
#include "../effect.h"
#include "../vertex.h"
#include "vec3.h"
#include <stdint.h>

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .color = ((type *)(var)->sd) }

typedef struct {
  vec3 pos;
  uint32_t *color;
} mvertex;

static uint32_t create_pixel(void *data, const vertex *inv) {
  UNUSED(data);
  mvertex in = get_sd(inv, uint32_t);

  return *in.color;
}

pixel_shader solid_shader_create() {
  return (pixel_shader){
      .create_pixel = create_pixel,
  };
}
