#ifndef EFFECT_H
#define EFFECT_H

#include "vertex.h"
#include <stdint.h>

typedef struct pixel_shader {
  uint32_t (*create_pixel)(void *data, const vertex *in);
  void *shader_data;
} pixel_shader;

typedef struct {
  pixel_shader ps;
} effect;

typedef struct pixel_shader_data pixel_shader_data;
pixel_shader texture_shader_create();
void texture_shader_bind_texture(pixel_shader_data *px, const char *file_name);
pixel_shader color_shader_create();

#endif
