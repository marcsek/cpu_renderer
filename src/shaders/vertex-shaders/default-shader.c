#include "../effect.h"

typedef struct {
  mat4 tranformation;
} default_shader_data;

static void bind_transformation(vertex_shader *vs, const mat4 in) {
  default_shader_data *sd = (default_shader_data *)vs->shader_data;
  sd->tranformation = in;
}

static vertex transform(void *data, vertex *in) {
  default_shader_data *sd = (default_shader_data *)data;

  vertex new_vertex = vertex_copy(in);

  vec4 pos_rot = mat4_mult_vec4(&sd->tranformation, &in->pos);

  new_vertex.pos = pos_rot;

  return new_vertex;
}

vertex_shader default_vertex_create() {
  default_shader_data *d = malloc(sizeof(default_shader_data));
  return (vertex_shader){
      .bind_transformation = bind_transformation,
      .transform = transform,
      .shader_data = d,
  };
}
