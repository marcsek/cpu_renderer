#include "../effect.h"
#include "mat4.h"
#include <stdio.h>

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

static mat4 get_proj(struct vertex_shader *vs) {
  default_shader_data *sd = (default_shader_data *)vs->shader_data;
  return sd->proj;
}

static vertex transform(void *data, vertex *in) {
  default_shader_data *sd = (default_shader_data *)data;

  vertex new_vertex = vertex_copy(in);

  vec4 pos_rot = mat4_mult_vec4(&sd->world_proj, &in->pos);

  new_vertex.pos = pos_rot;

  return new_vertex;
}

vertex_shader default_vertex_create() {
  default_shader_data *d = malloc(sizeof(default_shader_data));
  return (vertex_shader){
      .bind_proj = bind_proj,
      .bind_world = bind_world,
      .get_proj = get_proj,
      .transform = transform,
      .shader_data = d,
  };
}
