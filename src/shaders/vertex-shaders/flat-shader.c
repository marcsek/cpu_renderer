#include "../effect.h"
#include "../vertex.h"
#include "vec3.h"
#include <stdlib.h>

typedef struct {
  mat4 world;
  mat4 proj;
  mat4 world_proj;
  vec3 dir;
} flat_shader_data;

static vec3 diffuse = {1.0f, 1.0f, 1.0f};
static vec3 ambient = {0.1f, 0.1f, 0.1f};
static vec3 lcolor = {0.85f, 0.85f, 1.0f};

static void bind_world(vertex_shader *vs, const mat4 in) {
  flat_shader_data *sd = (flat_shader_data *)vs->shader_data;
  sd->world = in;
  sd->world_proj = mat4_mult_mat4(&sd->world, &sd->proj);
}

static void bind_proj(vertex_shader *vs, const mat4 in) {
  flat_shader_data *sd = (flat_shader_data *)vs->shader_data;
  sd->proj = in;
  sd->world_proj = mat4_mult_mat4(&sd->world, &sd->proj);
}

static vertex transform(void *data, vertex *in) {
  flat_shader_data *sd = (flat_shader_data *)data;
  normal_vertex *in_n = (normal_vertex *)in;

  vec4 pos_rot = mat4_mult_vec4(&sd->world_proj, &in->pos);

  vec3 *color = malloc(sizeof(vec3));
  // uint32_t *color = malloc(sizeof(uint32_t));

  vec3 d = vec3_copy(&diffuse);
  vec4 angle = mat4_mult_vec4(&sd->world_proj, in_n->normal);

  vec3 angle3 = (vec3){angle.x, angle.y, angle.z};

  vec3_mult_s(&angle3, -1.0f);
  float theta = vec3_dot_prod(&angle3, &sd->dir);
  vec3_mult_s(&d, MAX(0.0f, theta));

  vec3 hadamard = vec3_copy(&d);
  vec3_add(&hadamard, &ambient);
  vec3_mult(&hadamard, &lcolor);

  vec3 c = vec3_saturate(&hadamard);
  vec3_mult_s(&c, 255.0f);

  *color = c;
  //*color = MFB_ARGB(0xFF, c.x, c.y, c.z);

  return color_vertex_create(pos_rot, color);
}

void flat_vertex_bind_dir(void *data, const vec3 *dir_in) {
  ((flat_shader_data *)data)->dir = vec3_normalized(dir_in);
}

vertex_shader flat_vertex_create() {
  flat_shader_data *d = malloc(sizeof(flat_shader_data));
  return (vertex_shader){
      .bind_world = bind_world,
      .bind_proj = bind_proj,
      .transform = transform,
      .shader_data = d,
  };
}
