#include "../pipeline.h"
#include "../shapes/shapes.h"
#include "scene.h"
#include <MiniFB.h>

static pipeline pip;
static pipeline l_pip;
static vec3 rotation = {0.0f, 0.0f, 0.0f};
static float z_offset = 2.0f;
static indexed_triangle_list v;

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

  mat3 rot_x = mat3_rotationX(rotation.x);
  mat3 rot_y = mat3_rotationY(rotation.y);
  mat3 rot_z = mat3_rotationZ(rotation.z);
  mat3 rot_matrix = mat3_mult_mat3(&rot_z, &rot_y);
  rot_matrix = mat3_mult_mat3(&rot_matrix, &rot_x);

  const vec3 trans = {.z = z_offset};

  pip.effect.vs.bind_rotation(&pip.effect.vs, rot_matrix);
  pip.effect.vs.bind_translation(&pip.effect.vs, trans);
  pipeline_draw(&pip, &v);
}

scene test_cube_create(renderer *rn) {
  v = sphere_get_plain(1.0f, 12, 24, vertex_default_create);

  pixel_shader ps = default_pixel_create();
  vertex_shader vs = default_vertex_create();
  geometry_shader gs = default_geometry_create();

  effect ef = (effect){ps, vs, gs};

  pip = pipeline_create(rn, ef);

  return (scene){
      .update = update,
      .render = render,
  };
}
