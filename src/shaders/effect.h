#ifndef EFFECT_H
#define EFFECT_H

#include "../triangle.h"
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

typedef struct geometry_shader {
  triangle (*process)(void *data, const vertex *in1, const vertex *in2,
                      const vertex *in3, unsigned int index);
  void *shader_data;
} geometry_shader;

typedef struct {
  pixel_shader ps;
  vertex_shader vs;
  geometry_shader gs;
} effect;

typedef struct pixel_shader_data pixel_shader_data;
typedef struct vertex_shader_data vertex_shader_data;
typedef struct geomtry_shader_data geometry_shader_data;

// Pixel shaders
pixel_shader default_pixel_create();
void pixel_shader_bind_color(void *px, const vec3 *color_in);

pixel_shader texture_shader_create();
void texture_shader_bind_texture(pixel_shader_data *px, const char *file_name);

pixel_shader color_shader_create();
pixel_shader solid_shader_create();

// Vertex shaders
vertex_shader default_vertex_create();
vertex_shader position_vertex_create();

vertex_shader wawe_vertex_create();
void wawe_vertex_set_time(void *d, float time);

vertex_shader flat_vertex_create();
void flat_vertex_bind_dir(void *data, const vec3 *dir_in);

vertex_shader point_vertex_create();
void point_vertex_bind_pos(void *data, const vec3 *dir_in);

// Geometry shaders
geometry_shader default_geometry_create();

geometry_shader sface_geometry_create();
void sface_geo_bind_colors(void *data, uint32_t *colors);

#endif
