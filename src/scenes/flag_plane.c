#include "../pipeline.h"
#include "../shapes/shapes.h"
#include "scene.h"
#include "vector.h"
#include <MiniFB.h>
#include <stdio.h>

static pipeline pip;
static vec3 rotation = {0.0f, 0.0f, 0.0f};
static float z_offset = 2.0f;
static indexed_triangle_list v;
static float time = 0.0f;

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
  time += dt;
}

static void render(renderer *rn) {
  pipeline_begin_frame(&pip);

  mat3 rot_x = mat3_rotationX(rotation.x);
  mat3 rot_y = mat3_rotationY(rotation.y);
  mat3 rot_z = mat3_rotationZ(rotation.z);
  mat3 rot_matrix = mat3_mult_mat3(&rot_z, &rot_y);
  rot_matrix = mat3_mult_mat3(&rot_matrix, &rot_x);

  const vec3 trans = {.z = z_offset};

  pip.effect.vs.bind_rotation(&pip.effect.vs, rot_matrix);
  pip.effect.vs.bind_translation(&pip.effect.vs, trans);
  wawe_vertex_set_time(pip.effect.vs.shader_data, time);
  pipeline_draw(&pip, &v);
}

scene scene_flag_plane_create(renderer *rn) {
  v = plain_create_skinned(20, 1.0f);
  printf("%lu\n", vector_get_size(v.vertices));

  pixel_shader ps = texture_shader_create();
  texture_shader_bind_texture(ps.shader_data, "assets/sunf.png");

  vertex_shader vs = wawe_vertex_create();
  effect ef = (effect){ps, vs};
  pip = pipeline_create(rn, ef);

  return (scene){
      .update = update,
      .render = render,
  };
}
