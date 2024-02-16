#include "../app.h"
#include "../pipeline.h"
#include "mat3.h"
#include "scene.h"
#include <MiniFB.h>

static pipeline pip;
static vec3 rotation = {0.0f, 0.0f, 0.0f};
static vec3 rotation_c = {0.0f, 0.0f, 0.0f};
static vec3 light_dir = {10.2f, -0.5f, 1.0f};
static float z_offset = 2.0f;
static indexed_triangle_list v;
static z_buffer zb;

static void update(keyboard *kbd, double dt) {
  if (kbd_key_is_pressed(kbd, KB_KEY_Q)) {
    rotation.x += PI * dt;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_W)) {
    rotation.y += PI * dt;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_E)) {
    rotation.z += PI * dt;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_A)) {
    z_offset += 1.5f * dt;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_D)) {
    z_offset -= 1.5f * dt;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_Y)) {
    rotation_c.x += PI * dt;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_U)) {
    rotation_c.y += PI * dt;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_I)) {
    rotation_c.z += PI * dt;
  }
}

static void render() {
  pipeline_begin_frame(&pip);

  mat3 rot_x = mat3_rotationX(rotation.x);
  mat3 rot_y = mat3_rotationY(rotation.y);
  mat3 rot_z = mat3_rotationZ(rotation.z);
  mat3 rot_matrix = mat3_mult_mat3(&rot_z, &rot_y);
  rot_matrix = mat3_mult_mat3(&rot_matrix, &rot_x);

  mat3 rot_c_x = mat3_rotationX(rotation_c.x);
  mat3 rot_c_y = mat3_rotationY(rotation_c.y);
  mat3 rot_c_z = mat3_rotationZ(rotation_c.z);
  mat3 rot_c_matrix = mat3_mult_mat3(&rot_c_z, &rot_c_y);
  rot_c_matrix = mat3_mult_mat3(&rot_c_matrix, &rot_c_x);

  const vec3 trans = {.z = z_offset};
  const vec3 light_pos = mat3_mult_vec3(&rot_c_matrix, &light_dir);

  pip.effect.vs.bind_rotation(&pip.effect.vs, rot_matrix);
  pip.effect.vs.bind_translation(&pip.effect.vs, trans);
  flat_vertex_bind_dir(pip.effect.vs.shader_data, &light_pos);
  pipeline_draw(&pip, &v);
}

scene scene_bunny_create(renderer *rn) {
  v = itd_load_normals("assets/suzanne.obj", normal_vertex_create);

  pixel_shader ps = color_shader_create();
  vertex_shader vs = flat_vertex_create();
  geometry_shader gs = default_geometry_create();

  effect ef = (effect){ps, vs, gs};

  zb = z_buffer_create(WINDOW_WIDTH, WINDOW_HEIGHT);

  pip = pipeline_create(rn, ef, &zb);

  return (scene){
      .update = update,
      .render = render,
  };
}
