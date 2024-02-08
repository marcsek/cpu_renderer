#include "../app.h"
#include "../pipeline.h"
#include "../screen_transformer.h"
#include "../shapes/shapes.h"
#include "scene.h"
#include <MiniFB.h>

pipeline pip;
screen_transformer st;
static vec3 rotation = {0.0f, 0.0f, 0.0f};
static float z_offset = 2.0f;
static indexed_triangle_list_tex vt;

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

static void render(renderer *rn) {
  mat3 rot_x = mat3_rotationX(rotation.x);
  mat3 rot_y = mat3_rotationY(rotation.y);
  mat3 rot_z = mat3_rotationZ(rotation.z);
  mat3 rot_matrix = mat3_mult_mat3(&rot_z, &rot_y);
  rot_matrix = mat3_mult_mat3(&rot_matrix, &rot_x);

  const vec3 trans = {.z = z_offset};

  pipeline_bind_rotation(&pip, &rot_matrix);
  pipeline_bind_translation(&pip, &trans);
  pipeline_draw(&pip, &vt);
}

scene scene_solid_cube_create(renderer *rn) {
  st = st_create(WINDOW_WIDTH, WINDOW_HEIGHT);
  shape c = cube_create(1.0f);
  vt = c.get_skinned(&c);
  pip = pipeline_create(rn);
  pipeline_bind_texture(&pip, "assets/office_skin.png");

  return (scene){
      .update = update,
      .render = render,
  };
}
