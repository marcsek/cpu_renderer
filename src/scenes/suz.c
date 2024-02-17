#include "../app.h"
#include "../pipeline.h"
#include "mat3.h"
#include "scene.h"
#include <MiniFB.h>

static pipeline pip;
static pipeline l_pip;
static vec3 rotation = {0.0f, 0.0f, 0.0f};
static vec3 light_pos = {0.0f, 0.0f, 0.6f};
static float z_offset = 2.0f;
static indexed_triangle_list v;
static indexed_triangle_list l_v;
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
  if (kbd_key_is_pressed(kbd, KB_KEY_I)) {
    light_pos.z += 1.5f * dt;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_K)) {
    light_pos.z -= 1.5f * dt;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_J)) {
    light_pos.x -= 1.5f * dt;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_L)) {
    light_pos.x += 1.5f * dt;
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

  vec4 pos4 = {light_pos.x, light_pos.y, light_pos.z, 1.0f};
  pip.effect.vs.bind_transformation(&pip.effect.vs, transformation);
  point_vertex_bind_pos(pip.effect.vs.shader_data, &pos4);
  pipeline_draw(&pip, &v);

  mat4 ident = mat4_identity();
  mat4 l_translation = mat4_translation(&light_pos);
  mat4 l_transformation = mat4_mult_mat4(&l_translation, &ident);
  l_pip.effect.vs.bind_transformation(&l_pip.effect.vs, l_transformation);

  pipeline_draw(&l_pip, &l_v);
}

scene scene_suz_create(renderer *rn) {
  v = itd_load_normals("assets/suzanne.obj", normal_vertex_create);
  l_v = sphere_get_plain(0.05f, 12, 24, vertex_default_create);

  pixel_shader ps = color_shader_create();
  vertex_shader vs = point_vertex_create();
  geometry_shader gs = default_geometry_create();

  pixel_shader l_ps = default_pixel_create();
  vertex_shader l_vs = default_vertex_create();
  geometry_shader l_gs = default_geometry_create();

  vec3 light_color = (vec3){255.0f, 255.0f, 255.0f};
  pixel_shader_bind_color(l_ps.shader_data, &light_color);

  zb = z_buffer_create(WINDOW_WIDTH, WINDOW_HEIGHT);

  effect ef = (effect){ps, vs, gs};
  pip = pipeline_create(rn, ef, &zb);

  effect l_ef = (effect){l_ps, l_vs, l_gs};
  l_pip = pipeline_create(rn, l_ef, &zb);

  return (scene){
      .update = update,
      .render = render,
  };
}
