#include "../effect.h"
#include "../vertex.h"
#include "essentials.h"
#include "vec3.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  mat4 world;
  mat4 proj;
  mat4 world_proj;
  vec4 light_pos;
} point_shader_data;

static vec3 diffuse = {1.0f, 1.0f, 1.0f};
static vec3 ambient = {0.1f, 0.1f, 0.1f};
static vec3 lcolor = {0.85f, 0.85f, 1.0f};

static float linear_atten = 1.0f;
static float quadracit_atten = 2.619f;
static float const_atten = 0.382f;

static void bind_world(vertex_shader *vs, const mat4 in) {
  point_shader_data *sd = (point_shader_data *)vs->shader_data;
  sd->world = in;
  sd->world_proj = mat4_mult_mat4(&sd->world, &sd->proj);
}

static void bind_proj(vertex_shader *vs, const mat4 in) {
  point_shader_data *sd = (point_shader_data *)vs->shader_data;
  sd->proj = in;
  sd->world_proj = mat4_mult_mat4(&sd->world, &sd->proj);
}

static mat4 get_proj(struct vertex_shader *vs) {
  point_shader_data *sd = (point_shader_data *)vs->shader_data;
  return sd->proj;
}

static vertex transform(void *data, vertex *in) {
  point_shader_data *sd = (point_shader_data *)data;
  normal_vertex *in_n = (normal_vertex *)in;
  UNUSED(in_n);

  vec4 pos_rot = mat4_mult_vec4(&sd->world_proj, &in->pos);
  vec4 world_pos = mat4_mult_vec4(&sd->world, &in->pos);

  vec3 *color = malloc(sizeof(vec3));

  vec3 v_to_l = (vec3){sd->light_pos.x, sd->light_pos.y, sd->light_pos.z};
  vec3 pos_rot_3 = (vec3){world_pos.x, world_pos.y, world_pos.z};
  vec3_subs(&v_to_l, &pos_rot_3);
  float dist = vec3_length(&v_to_l);

  vec3 dir = vec3_copy(&v_to_l);
  vec3_div_s(&dir, dist);

  const float atten = 1.0f / (const_atten + linear_atten * dist +
                              quadracit_atten * dist * dist);

  vec3 d = vec3_copy(&diffuse);
  vec3_mult_s(&d, atten);

  vec4 angle = mat4_mult_vec4(&sd->world, in_n->normal);
  vec3 angle3 = (vec3){angle.x, angle.y, angle.z};

  // vec3_mult_s(&angle3, -1.0f);
  float theta = vec3_dot_prod(&angle3, &dir);
  vec3_mult_s(&d, MAX(0.0f, theta));

  vec3 hadamard = vec3_copy(&d);
  vec3_add(&hadamard, &ambient);
  vec3_mult(&hadamard, &lcolor);

  vec3 c = vec3_saturate(&hadamard);
  vec3_mult_s(&c, 255.0f);

  *color = c;

  return color_vertex_create(pos_rot, color);
}

void point_vertex_bind_pos(void *data, const vec4 *pos_in) {
  ((point_shader_data *)data)->light_pos = *pos_in;
}

vertex_shader point_vertex_create() {
  point_shader_data *d = malloc(sizeof(point_shader_data));
  return (vertex_shader){
      .bind_proj = bind_proj,
      .bind_world = bind_world,
      .get_proj = get_proj,
      .transform = transform,
      .shader_data = d,
  };
}
