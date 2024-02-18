#include "../effect.h"
#include "../vertex.h"
#include "vec3.h"
#include <math.h>
#include <stdlib.h>

typedef struct {
  mat4 world;
  mat4 proj;
  mat4 world_proj;
} default_shader_data;

static void bind_world(vertex_shader *vs, const mat4 in) {
  default_shader_data *sd = (default_shader_data *)vs->shader_data;
  sd->world = in;
  sd->world_proj = mat4_mult_mat4(&sd->world, &sd->proj);
}

static void bind_proj(vertex_shader *vs, const mat4 in) {
  default_shader_data *sd = (default_shader_data *)vs->shader_data;
  sd->proj = in;
  sd->world_proj = mat4_mult_mat4(&sd->world, &sd->proj);
}

static vertex transform(void *data, vertex *in) {
  default_shader_data *sd = (default_shader_data *)data;

  vec4 pos_rot = mat4_mult_vec4(&sd->world_proj, &in->pos);

  vec3 *color = malloc(sizeof(vec3));
  *color =
      (vec3){.x = fabs(pos_rot.x), .y = fabs(pos_rot.y), .z = fabs(pos_rot.z)};
  vec3_mult_s(color, 255.0f);

  vertex new_vertex = color_vertex_create(pos_rot, color);

  return new_vertex;
}

vertex_shader position_vertex_create() {
  default_shader_data *d = malloc(sizeof(default_shader_data));
  return (vertex_shader){
      .bind_proj = bind_proj,
      .bind_world = bind_world,
      .transform = transform,
      .shader_data = d,
  };
}
