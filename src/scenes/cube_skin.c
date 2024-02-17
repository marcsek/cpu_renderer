#include "../app.h"
#include "../pipeline.h"
#include "../shapes/shapes.h"
#include "scene.h"
#include <MiniFB.h>

static pipeline pip;
static vec3 rotation = {0.0f, 0.0f, 0.0f};
static float z_offset = 2.0f;
static indexed_triangle_list vt;
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

  pip.effect.vs.bind_transformation(&pip.effect.vs, transformation);
  pipeline_draw(&pip, &vt);
}

scene scene_cube_skin_create(renderer *rn) {
  vt = cube_create_skinned(1.0f, tex_vertex_create);

  pixel_shader ps = texture_shader_create();
  texture_shader_bind_texture(ps.shader_data, "assets/office_skin.png");
  vertex_shader vs = default_vertex_create();
  geometry_shader gs = default_geometry_create();

  effect ef = (effect){ps, vs, gs};

  zb = z_buffer_create(WINDOW_WIDTH, WINDOW_HEIGHT);

  pip = pipeline_create(rn, ef, &zb);

  return (scene){
      .update = update,
      .render = render,
  };
}
