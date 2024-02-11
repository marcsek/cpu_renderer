#ifndef EFFECT_H
#define EFFECT_H

#include "vertex.h"
#include <stdint.h>

typedef struct pixel_shader {
  uint32_t (*create_pixel)(void *data, const vertex *in);
  void *shader_data;
} pixel_shader;

typedef struct vertex_shader {
  vertex (*transform)(void *data, vertex *in);
  void (*bind_translation)(struct vertex_shader *vs, const vec3 in);
  void (*bind_rotation)(struct vertex_shader *vs, const mat3 in);
  void *shader_data;
} vertex_shader;

typedef struct {
  pixel_shader ps;
  vertex_shader vs;
} effect;

typedef struct pixel_shader_data pixel_shader_data;
typedef struct vertex_shader_data vertex_shader_data;

// Pixel shaders
pixel_shader texture_shader_create();
void texture_shader_bind_texture(pixel_shader_data *px, const char *file_name);

pixel_shader color_shader_create();

// Vertex shaders
vertex_shader default_vertex_create();
vertex_shader position_vertex_create();

vertex_shader wawe_vertex_create();
void wawe_vertex_set_time(void *d, float time);

#endif
