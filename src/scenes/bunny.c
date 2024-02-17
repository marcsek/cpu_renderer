#include "../app.h"
#include "../pipeline.h"
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
  mat4 rot_x = mat4_rotationX(rotation.x);
  mat4 rot_y = mat4_rotationY(rotation.y);
  mat4 rot_z = mat4_rotationZ(rotation.z);
  mat4 rot_matrix = mat4_mult_mat4(&rot_z, &rot_y);
  rot_matrix = mat4_mult_mat4(&rot_matrix, &rot_x);
  mat4 translation = mat4_translation(&(vec3){.z = z_offset});
  mat4 transformation = mat4_mult_mat4(&translation, &rot_matrix);

  mat4 rot_c_x = mat4_rotationX(rotation_c.x);
  mat4 rot_c_y = mat4_rotationY(rotation_c.y);
  mat4 rot_c_z = mat4_rotationZ(rotation_c.z);
  mat4 rot_c_matrix = mat4_mult_mat4(&rot_c_z, &rot_c_y);
  rot_c_matrix = mat4_mult_mat4(&rot_c_matrix, &rot_c_x);

  const vec4 light_dir4 = (vec4){light_dir.x, light_dir.y, light_dir.z, 1.0f};
  const vec4 light_pos = mat4_mult_vec4(&rot_c_matrix, &light_dir4);
  const vec3 light_pos3 = (vec3){light_pos.x, light_pos.y, light_pos.z};

  pip.effect.vs.bind_transformation(&pip.effect.vs, transformation);
  flat_vertex_bind_dir(pip.effect.vs.shader_data, &light_pos3);
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
