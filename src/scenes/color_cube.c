#include "../app.h"
#include "../pipeline.h"
#include "../shapes/shapes.h"
#include "scene.h"
#include "vector.h"
#include <MiniFB.h>

static pipeline pip;
static vec3 rotation = {0.0f, 0.0f, 0.0f};
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

scene scene_color_cube_create(renderer *rn) {
  v = cube_create_plain(1.0f, color_vertex_create);
  vertex **d = (vertex **)vector_get_data(v.vertices);

  for (size_t i = 0; i < 8; i++) {
    vec3 *c = malloc(sizeof(vec3));
    d[i]->sd = c;
  }

  *(vec3 *)d[0]->sd = (vec3){0x00, 0x00, 0x00};
  *(vec3 *)d[1]->sd = (vec3){0xFF, 0x00, 0x00};
  *(vec3 *)d[2]->sd = (vec3){0x00, 0xFF, 0x00};
  *(vec3 *)d[3]->sd = (vec3){0xFF, 0xFF, 0x00};
  *(vec3 *)d[4]->sd = (vec3){0x00, 0x00, 0xFF};
  *(vec3 *)d[5]->sd = (vec3){0xFF, 0x00, 0xFF};
  *(vec3 *)d[6]->sd = (vec3){0x00, 0xFF, 0xFF};
  *(vec3 *)d[7]->sd = (vec3){0xFF, 0xFF, 0xFF};

  pixel_shader ps = color_shader_create();
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
