#include "../effect.h"

typedef struct {
  mat3 rotation;
  vec3 translation;
} default_shader_data;

static void bind_translation(vertex_shader *vs, const vec3 in) {
  default_shader_data *sd = (default_shader_data *)vs->shader_data;
  sd->translation = in;
}

static void bind_rotation(vertex_shader *vs, const mat3 in) {
  default_shader_data *sd = (default_shader_data *)vs->shader_data;
  sd->rotation = in;
}

static vertex transform(void *data, vertex *in) {
  default_shader_data *sd = (default_shader_data *)data;

  vertex new_vertex = vertex_copy(in);

  vec3 pos_rot = mat3_mult_vec3(&sd->rotation, &in->pos);
  vec3_add(&pos_rot, &sd->translation);

  new_vertex.pos = pos_rot;

  return new_vertex;
}

vertex_shader default_vertex_create() {
  default_shader_data *d = malloc(sizeof(default_shader_data));
  return (vertex_shader){
      .bind_translation = bind_translation,
      .bind_rotation = bind_rotation,
      .transform = transform,
      .shader_data = d,
  };
}
