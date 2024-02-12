#include "../pipeline.h"
#include "../shapes/shapes.h"
#include "scene.h"
#include <MiniFB.h>
#include <stdint.h>
#include <stdlib.h>

static pipeline pip;
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

scene scene_solid_face_create(renderer *rn) {
  v = cube_create_plain(1.0f, vertex_default_create);

  uint32_t *colors = malloc(6 * sizeof(uint32_t));

  colors[0] = 0xFF1f3c40;
  colors[1] = 0xFFe35000;
  colors[2] = 0xFFe1ae0f;
  colors[3] = 0xFF3d8ebf;
  colors[4] = 0xFFa3a32f;
  colors[5] = 0xFF77dd83;

  pixel_shader ps = solid_shader_create();
  vertex_shader vs = default_vertex_create();
  geometry_shader gs = sface_geometry_create();
  sface_geo_bind_colors(gs.shader_data, colors);

  effect ef = (effect){ps, vs, gs};
  pip = pipeline_create(rn, ef);

  return (scene){
      .update = update,
      .render = render,
  };
}
