#include "../../surface.h"
#include "../effect.h"
#include "../vertex.h"
#include "vec2.h"
#include <stdlib.h>

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .tc = ((type *)(var)->sd) }

typedef struct pixel_shader_data {
  surface sf;
  float tex_xclamp;
  float tex_yclamp;
} texture_shader_data;

static uint32_t create_pixel(void *data, const vertex *inv) {
  texture_shader_data *px = (texture_shader_data *)data;
  tex_vertex in = get_sd(inv, vec2);

  return surface_get_pixel(
      &px->sf,
      (int)MIN(in.tc->x * ((float)px->sf.width) + 0.5f, px->tex_xclamp),
      (int)MIN(in.tc->y * ((float)px->sf.height) + 0.5f, px->tex_yclamp));
}

void texture_shader_bind_texture(texture_shader_data *px,
                                 const char *file_name) {
  px->sf = surface_from_file(file_name);
  px->tex_xclamp = ((float)px->sf.width) - 1.0f;
  px->tex_yclamp = ((float)px->sf.height) - 1.0f;
}

pixel_shader texture_shader_create() {
  texture_shader_data *d = malloc(sizeof(texture_shader_data));
  return (pixel_shader){
      .create_pixel = create_pixel,
      .shader_data = d,
  };
}
