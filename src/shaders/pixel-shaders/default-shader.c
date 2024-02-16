#include "../effect.h"
#include "../vertex.h"
#include "MiniFB.h"
#include <stdint.h>

typedef struct default_pix_shader_data {
  vec3 color;
} default_pix_shader_data;

static uint32_t create_pixel(void *data, const vertex *inv) {
  UNUSED(data);
  UNUSED(inv);

  default_pix_shader_data *sd = (default_pix_shader_data *)data;

  return MFB_ARGB(0xFF, sd->color.x, sd->color.y, sd->color.z);
}

void pixel_shader_bind_color(void *px, const vec3 *color_in) {
  default_pix_shader_data *sd = (default_pix_shader_data *)px;
  sd->color = *color_in;
}

pixel_shader default_pixel_create() {
  default_pix_shader_data *d = malloc(sizeof(default_pix_shader_data));
  d->color = (vec3){255.0f, 255.0f, 255.0f};
  return (pixel_shader){
      .create_pixel = create_pixel,
      .shader_data = d,
  };
}
