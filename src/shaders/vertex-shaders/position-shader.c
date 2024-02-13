#include "../effect.h"
#include "../vertex.h"
#include "vec3.h"
#include <math.h>
#include <stdlib.h>

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
  vec3 pos_rot = mat3_mult_vec3(&sd->rotation, &in->pos);
  vec3_add(&pos_rot, &sd->translation);

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
      .bind_translation = bind_translation,
      .bind_rotation = bind_rotation,
      .transform = transform,
      .shader_data = d,
  };
}
