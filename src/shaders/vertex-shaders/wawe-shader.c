#include "../effect.h"
#include "vec3.h"
#include <math.h>
#include <stdlib.h>

typedef struct {
  mat3 rotation;
  vec3 translation;
  float time;
} wawe_shader_data;

static void bind_translation(vertex_shader *vs, const vec3 in) {
  wawe_shader_data *sd = (wawe_shader_data *)vs->shader_data;
  sd->translation = in;
}

static void bind_rotation(vertex_shader *vs, const mat3 in) {
  wawe_shader_data *sd = (wawe_shader_data *)vs->shader_data;
  sd->rotation = in;
}

static vertex transform(void *data, vertex *in) {
  wawe_shader_data *sd = (wawe_shader_data *)data;

  vertex new_vertex = vertex_copy(in);

  vec3 pos_rot = mat3_mult_vec3(&sd->rotation, &in->pos);
  vec3_add(&pos_rot, &sd->translation);

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
      .bind_translation = bind_translation,
      .bind_rotation = bind_rotation,
      .transform = transform,
      .shader_data = d,
  };
}
