#include "../effect.h"
#include "../vertex.h"
#include "essentials.h"
#include "mat3.h"
#include "vec3.h"
#include <stdlib.h>

typedef struct {
  mat3 rotation;
  vec3 translation;
  vec3 light_pos;
} point_shader_data;

static vec3 diffuse = {1.0f, 1.0f, 1.0f};
static vec3 ambient = {0.1f, 0.1f, 0.1f};
static vec3 lcolor = {0.85f, 0.85f, 1.0f};

static float linear_atten = 1.0f;
static float quadracit_atten = 2.619f;
static float const_atten = 0.382f;

static void bind_translation(vertex_shader *vs, const vec3 in) {
  point_shader_data *sd = (point_shader_data *)vs->shader_data;
  sd->translation = in;
}

static void bind_rotation(vertex_shader *vs, const mat3 in) {
  point_shader_data *sd = (point_shader_data *)vs->shader_data;
  sd->rotation = in;
}

static vertex transform(void *data, vertex *in) {
  point_shader_data *sd = (point_shader_data *)data;
  normal_vertex *in_n = (normal_vertex *)in;

  vec3 pos_rot = mat3_mult_vec3(&sd->rotation, &in->pos);
  vec3_add(&pos_rot, &sd->translation);

  vec3 *color = malloc(sizeof(vec3));

  vec3 v_to_l = vec3_copy(&sd->light_pos);
  vec3_subs(&v_to_l, &pos_rot);
  float dist = vec3_length(&v_to_l);

  vec3 dir = vec3_copy(&v_to_l);
  vec3_div_s(&dir, dist);

  const float atten = 1.0f / (const_atten + linear_atten * dist +
                              quadracit_atten * dist * dist);

  vec3 d = vec3_copy(&diffuse);
  vec3_mult_s(&d, atten);

  vec3 angle = mat3_mult_vec3(&sd->rotation, in_n->normal);

  // vec3_mult_s(&angle, 1.0f);
  float theta = vec3_dot_prod(&angle, &dir);
  vec3_mult_s(&d, MAX(0.0f, theta));

  vec3 hadamard = vec3_copy(&d);
  vec3_add(&hadamard, &ambient);
  vec3_mult(&hadamard, &lcolor);

  vec3 c = vec3_saturate(&hadamard);
  vec3_mult_s(&c, 255.0f);

  *color = c;

  return color_vertex_create(pos_rot, color);
}

void point_vertex_bind_pos(void *data, const vec3 *pos_in) {
  ((point_shader_data *)data)->light_pos = *pos_in;
}

vertex_shader point_vertex_create() {
  point_shader_data *d = malloc(sizeof(point_shader_data));
  return (vertex_shader){
      .bind_translation = bind_translation,
      .bind_rotation = bind_rotation,
      .transform = transform,
      .shader_data = d,
  };
}
