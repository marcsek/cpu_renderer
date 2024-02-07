#include "../app.h"
#include "../helpers/helpers.h"
#include "../screen_transformer.h"
#include "../shapes/shapes.h"
#include "../texture-vertex.h"
#include "scene.h"
#include <MiniFB.h>

static shape c;
screen_transformer st;
surface sf;
static vec3 rotation = {0.0f, 0.0f, 0.0f};
static float z_offset = 2.0f;

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

  indexed_triangle_list_tex triangles = c.get_triangles_tex(&c);
  tex_vertex **verts = (tex_vertex **)vector_get_data(triangles.vertices);

  for (size_t i = 0; i < vector_get_size(triangles.vertices); i++) {
    verts[i]->pos = mat3_mult_vec3(&rot_matrix, &verts[i]->pos);
    vec3_add(&verts[i]->pos, &(vec3){.z = z_offset});
  }

  for (size_t i = 0; i < 36; i += 3) {
    vec3 *v1 = &verts[triangles.indices[i]]->pos;
    vec3 v2_c = vec3_copy(&verts[triangles.indices[i + 1]]->pos);
    vec3 v3_c = vec3_copy(&verts[triangles.indices[i + 2]]->pos);

    vec3_subs(&v2_c, v1);
    vec3_subs(&v3_c, v1);
    vec3 cross = vec3_cross_prod(&v2_c, &v3_c);

    triangles.cull_flags[i / 3] =
        vec3_dot_prod(&cross, v1) > 0.0f ? true : false;
  }

  for (size_t i = 0; i < vector_get_size(triangles.vertices); i++) {
    st_transform(&st, &verts[i]->pos);
  }

  for (size_t i = 0; i < 36; i += 3) {
    int i1 = triangles.indices[i];
    int i2 = triangles.indices[i + 1];
    int i3 = triangles.indices[i + 2];
    tex_vertex *v1 = verts[i1];
    tex_vertex *v2 = verts[i2];
    tex_vertex *v3 = verts[i3];

    if (!triangles.cull_flags[i / 3]) {
      renderer_create_triangle_tex(rn, v1, v2, v3, &sf);
    }
  }
}

scene scene_cube_tex_create() {
  st = st_create(WINDOW_WIDTH, WINDOW_HEIGHT);
  c = cube_folded_create(1.0f);
  sf = surface_from_file("assets/sunf.png");

  return (scene){
      .update = update,
      .render = render,
  };
}
