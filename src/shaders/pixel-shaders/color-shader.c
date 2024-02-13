#include "../effect.h"
#include "../vertex.h"
#include "MiniFB.h"
#include "essentials.h"
#include "vec3.h"

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .color = ((type *)(var)->sd) }

static uint32_t create_pixel(void *data, const vertex *inv) {
  UNUSED(data);
  color_vertex in = get_sd(inv, vec3);

  return MFB_ARGB(0xFF, in.color->x, in.color->y, in.color->z);
}

pixel_shader color_shader_create() {
  return (pixel_shader){
      .create_pixel = create_pixel,
  };
}
