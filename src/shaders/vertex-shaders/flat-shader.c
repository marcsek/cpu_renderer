#include "../effect.h"
#include "../vertex.h"
#include "MiniFB.h"
#include "essentials.h"
#include "mat3.h"
#include "vec3.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  mat3 rotation;
  vec3 translation;
  vec3 dir;
} flat_shader_data;

static vec3 diffuse = {1.0f, 1.0f, 1.0f};
static vec3 ambient = {0.1f, 0.1f, 0.1f};
static vec3 lcolor = {0.85f, 0.85f, 1.0f};

static void bind_translation(vertex_shader *vs, const vec3 in) {
  flat_shader_data *sd = (flat_shader_data *)vs->shader_data;
  sd->translation = in;
}

static void bind_rotation(vertex_shader *vs, const mat3 in) {
  flat_shader_data *sd = (flat_shader_data *)vs->shader_data;
  sd->rotation = in;
}

static vertex transform(void *data, vertex *in) {
  flat_shader_data *sd = (flat_shader_data *)data;
  color_vertex *in_c = (color_vertex *)in;

  vec3 pos_rot = mat3_mult_vec3(&sd->rotation, &in->pos);
  vec3_add(&pos_rot, &sd->translation);

  vec3 *color = malloc(sizeof(vec3));
  // uint32_t *color = malloc(sizeof(uint32_t));

  vec3 d = vec3_copy(&diffuse);
  vec3 angle = mat3_mult_vec3(&sd->rotation, in_c->color);

  vec3_mult_s(&angle, -1.0f);
  float theta = vec3_dot_prod(&angle, &sd->dir);
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
      .bind_translation = bind_translation,
      .bind_rotation = bind_rotation,
      .transform = transform,
      .shader_data = d,
  };
}
