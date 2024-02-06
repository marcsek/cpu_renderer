#include "app.h"
#include "helpers/debug_info.h"
#include "keyboard.h"
#include "mat3.h"
#include "renderer/renderer.h"
#include "screen_transformer.h"
#include "shapes/shapes.h"
#include "vec3.h"
#include "vector.h"
#include "window.h"
#include <MiniFB.h>
#include <stdio.h>

static window wnd;
static renderer rn;

// **
screen_transformer st;
cube c;
vec3 rotation = {0.0f, 0.0f, 0.0f};
float z_offset = 2.0f;
// **

static void handle_keypress(double dt);
static void app_tick(double dt);
static void app_update();

int app_init() {
  struct mfb_window *mfb_window =
      mfb_open_ex(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, WF_RESIZABLE);

  if (!mfb_window)
    return -1;

  wnd = window_create(mfb_window);
  rn = renderer_create(WINDOW_WIDTH, WINDOW_HEIGHT);

  mfb_set_target_fps(TARGET_FPS);

  // **
  st = st_create(WINDOW_WIDTH, WINDOW_HEIGHT);
  c = cube_create(1.0f);
  // **

  return 0;
}

void app_mainloop() {
  struct mfb_timer *timer = mfb_timer_create();
  struct mfb_timer *tick_timer = mfb_timer_create();
  double total_tick_time = 0.0f;
  double delta = 1.0f;
  double frame_time = delta * 1000;
  double fps = 1.0f / delta;

  debug_header_add(" Frame time: %.3f ms", &frame_time);
  debug_header_add(" FPS: %.0f", &fps);

  do {
    int state;
    double dt = mfb_timer_delta(tick_timer);

    mfb_timer_now(timer);

    app_tick(dt);
    app_update();

    uint32_t *window_buffer = renderer_get_buffer(&rn);
    state = mfb_update_ex(wnd.mfb_window, window_buffer, WINDOW_WIDTH,
                          WINDOW_HEIGHT);

    if (state < 0) {
      window_destroy(&wnd);
      break;
    }

    delta = mfb_timer_delta(timer);
    frame_time = delta * 1000;
    fps = 1.0f / delta;

    debug_info_output();

  } while (mfb_wait_sync(wnd.mfb_window));

  debug_exit();
}

static void app_tick(double dt) { handle_keypress(dt); }

vector *copy_model(vector *model) {
  vector *new = vector_create_cf(vector_get_size(model), free);
  vec3 **data = (vec3 **)vector_get_data(model);

  for (size_t i = 0; i < vector_get_size(model); i++) {
    vec3 *v = malloc(sizeof(vec3));
    vec3 original = *data[i];
    v->x = original.x;
    v->y = original.y;
    v->z = original.z;
    vector_push_back(new, v);
  }
  return new;
}

const uint32_t colors[12] = {
    0xFFFF0000, 0xFF880000, 0xFFFF8800, 0xFFFFFF00, 0xFFFFFF88, 0xFFFFFFFF,
    0xFF88FFFF, 0xFF00FFFF, 0xFFFF88FF, 0xFFFF00FF, 0xFF8888FF, 0xFF8800FF,
};

static void app_update() {
  renderer_reset_buffer(&rn);

  mat3 rot_x = mat3_rotationX(rotation.x);
  mat3 rot_y = mat3_rotationY(rotation.y);
  mat3 rot_z = mat3_rotationZ(rotation.z);
  mat3 rot_matrix = mat3_mult_mat3(&rot_z, &rot_y);
  rot_matrix = mat3_mult_mat3(&rot_matrix, &rot_x);

  indexed_triangle_list triangles = cube_get_triangles(&c);
  vector *vertices_c = copy_model(triangles.vertices);
  vec3 **verts = (vec3 **)vector_get_data(vertices_c);

  for (size_t i = 0; i < vector_get_size(vertices_c); i++) {
    *verts[i] = mat3_mult_vec3(&rot_matrix, verts[i]);
    vec3_add(verts[i], &(vec3){.z = z_offset});
  }

  for (size_t i = 0; i < 36; i += 3) {
    vec3 *v1 = verts[triangles.indices[i]];
    vec3 v2_c = vec3_copy(verts[triangles.indices[i + 1]]);
    vec3 v3_c = vec3_copy(verts[triangles.indices[i + 2]]);

    vec3_subs(&v2_c, v1);
    vec3_subs(&v3_c, v1);
    vec3 cross = vec3_cross_prod(&v2_c, &v3_c);

    triangles.cull_flags[i / 3] =
        vec3_dot_prod(&cross, v1) > 0.0f ? true : false;
  }

  for (size_t i = 0; i < vector_get_size(vertices_c); i++) {
    st_transform(&st, verts[i]);
  }

  for (size_t i = 0; i < 36; i += 3) {
    int i1 = triangles.indices[i];
    int i2 = triangles.indices[i + 1];
    int i3 = triangles.indices[i + 2];
    vec3 *v1 = verts[i1];
    vec3 *v2 = verts[i2];
    vec3 *v3 = verts[i3];

    if (!triangles.cull_flags[i / 3]) {
      renderer_create_triangle(&rn, &(vec2){.x = v1->x, .y = v1->y},
                               &(vec2){.x = v2->x, .y = v2->y},
                               &(vec2){.x = v3->x, .y = v3->y}, colors[i / 3]);
    }
  }
}

static void handle_keypress(double dt) {
  if (kbd_key_is_pressed(wnd.kbd, KB_KEY_Q)) {
    rotation.x += 3.14 * dt;
  }
  if (kbd_key_is_pressed(wnd.kbd, KB_KEY_W)) {
    rotation.y += 3.14 * dt;
  }
  if (kbd_key_is_pressed(wnd.kbd, KB_KEY_E)) {
    rotation.z += 3.14 * dt;
  }
  if (kbd_key_is_pressed(wnd.kbd, KB_KEY_A)) {
    z_offset += 1.5f * dt;
  }
  if (kbd_key_is_pressed(wnd.kbd, KB_KEY_D)) {
    z_offset -= 1.5f * dt;
  }
}
