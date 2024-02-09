#ifndef EFFECT_H
#define EFFECT_H

#include "../surface.h"
#include "vertex.h"
#include <stdint.h>

typedef struct pixel_shader_data {
  surface sf;
  float tex_xclamp;
  float tex_yclamp;
} pixel_shader_data;

typedef struct pixel_shader {
  uint32_t (*create_pixel)(void *data, const vertex *in);
  void *shader_data;
} pixel_shader;

typedef struct {
  pixel_shader ps;
} effect;

pixel_shader tex_ps_create();
void pixel_bind_texture(pixel_shader_data *px, const char *file_name);

#endif
