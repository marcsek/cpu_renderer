#include "../effect.h"
#include <math.h>
#include <stdlib.h>

typedef struct {
  mat4 transformation;
  float time;
} wawe_shader_data;

static void bind_transformation(vertex_shader *vs, const mat4 in) {
  wawe_shader_data *sd = (wawe_shader_data *)vs->shader_data;
  sd->transformation = in;
}

static vertex transform(void *data, vertex *in) {
  wawe_shader_data *sd = (wawe_shader_data *)data;

  vertex new_vertex = vertex_copy(in);

  vec4 pos_rot = mat4_mult_vec4(&sd->transformation, &in->pos);

  pos_rot.y += 0.05f * sinf(sd->time * 5.0f + pos_rot.x * 10.0f);

  new_vertex.pos = pos_rot;

  return new_vertex;
}

void wawe_vertex_set_time(void *d, float time) {
  wawe_shader_data *wd = (wawe_shader_data *)d;
  wd->time = time;
}

vertex_shader wawe_vertex_create() {
  wawe_shader_data *d = malloc(sizeof(wawe_shader_data));
  return (vertex_shader){
      .bind_transformation = bind_transformation,
      .transform = transform,
      .shader_data = d,
  };
}
