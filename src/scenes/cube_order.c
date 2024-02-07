#include "../app.h"
#include "../screen_transformer.h"
#include "../shapes/shapes.h"
#include "scene.h"
#include <MiniFB.h>

static shape c;
screen_transformer st;
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

static const uint32_t colors[12] = {
    0xFFFF0000, 0xFF880000, 0xFFFF8800, 0xFFFFFF00, 0xFFFFFF88, 0xFFFFFFFF,
    0xFF88FFFF, 0xFF00FFFF, 0xFFFF88FF, 0xFFFF00FF, 0xFF8888FF, 0xFF8800FF,
};

static void render(renderer *rn) {
  // Fixed cube
  {
    mat3 rot_x = mat3_rotationX(rotation.x);
    mat3 rot_y = mat3_rotationY(rotation.y + PI);
    mat3 rot_z = mat3_rotationZ(rotation.z);
    mat3 rot_matrix = mat3_mult_mat3(&rot_z, &rot_y);
    rot_matrix = mat3_mult_mat3(&rot_matrix, &rot_x);

    indexed_triangle_list_tex triangles = c.get_triangles_tex(&c);
    tex_vertex **verts = (tex_vertex **)vector_get_data(triangles.vertices);

    for (size_t i = 0; i < vector_get_size(triangles.vertices); i++) {
      verts[i]->pos = mat3_mult_vec3(&rot_matrix, &verts[i]->pos);
      vec3_add(&verts[i]->pos, &(vec3){.z = 2.0f});
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
      vec3 *v1 = &verts[i1]->pos;
      vec3 *v2 = &verts[i2]->pos;
      vec3 *v3 = &verts[i3]->pos;

      if (!triangles.cull_flags[i / 3]) {
        renderer_create_triangle(rn, &(vec2){.x = v1->x, .y = v1->y},
                                 &(vec2){.x = v2->x, .y = v2->y},
                                 &(vec2){.x = v3->x, .y = v3->y},
                                 colors[i / 3]);
      }
    }
  }
  // Moving cube
  {
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
      vec3 *v1 = &verts[i1]->pos;
      vec3 *v2 = &verts[i2]->pos;
      vec3 *v3 = &verts[i3]->pos;

      if (!triangles.cull_flags[i / 3]) {
        renderer_create_triangle(rn, &(vec2){.x = v1->x, .y = v1->y},
                                 &(vec2){.x = v2->x, .y = v2->y},
                                 &(vec2){.x = v3->x, .y = v3->y},
                                 colors[i / 3]);
      }
    }
  }
}

scene scene_cube_order_create() {
  st = st_create(WINDOW_WIDTH, WINDOW_HEIGHT);
  c = cube_create(1.0f);

  return (scene){
      .update = update,
      .render = render,
  };
}
