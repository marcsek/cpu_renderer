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

  mat3 rot_x = mat3_rotationX(rotation.x);
  mat3 rot_y = mat3_rotationY(rotation.y);
  mat3 rot_z = mat3_rotationZ(rotation.z);
  mat3 rot_matrix = mat3_mult_mat3(&rot_z, &rot_y);
  rot_matrix = mat3_mult_mat3(&rot_matrix, &rot_x);

  const vec3 trans = {.z = z_offset};

  pip.effect.vs.bind_rotation(&pip.effect.vs, rot_matrix);
  pip.effect.vs.bind_translation(&pip.effect.vs, trans);
  point_vertex_bind_pos(pip.effect.vs.shader_data, &light_pos);
  pipeline_draw(&pip, &v);

  pipeline_begin_frame(&l_pip);
  mat3 ident = mat3_identity();
  l_pip.effect.vs.bind_rotation(&l_pip.effect.vs, ident);
  l_pip.effect.vs.bind_translation(&l_pip.effect.vs, light_pos);
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

  effect ef = (effect){ps, vs, gs};
  pip = pipeline_create(rn, ef);

  effect l_ef = (effect){l_ps, l_vs, l_gs};
  l_pip = pipeline_create(rn, l_ef);

  return (scene){
      .update = update,
      .render = render,
  };
}
