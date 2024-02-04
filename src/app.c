#include "app.h"
#include "camera.h"
#include "coor_transformer.h"
#include "helpers/diagnostics.h"
#include "keyboard.h"
#include "renderer/renderer.h"
#include "world.h"
#include <MiniFB.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static struct mfb_window *window;
static keyboard *kbd;
static renderer rn;
static world wrld;
static coordinate_transformer ct;
static camera cm;

static void handle_keypress();
static void app_tick(double dt);
static void app_update();

static void keyboard_listener(struct mfb_window *window, mfb_key key,
                              mfb_key_mod mod, bool isPressed) {
  kbd_register_key_press(kbd, key, isPressed);
}

int app_init() {
  window = mfb_open_ex(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, WF_RESIZABLE);

  if (!window)
    return -1;

  rn = renderer_create(WINDOW_WIDTH, WINDOW_HEIGHT);
  ct = coor_tranformer_create(&rn);
  cm = camera_create(&ct);
  kbd = kbd_create();

  mfb_set_target_fps(TARGET_FPS);
  mfb_set_keyboard_callback(window, keyboard_listener);

  world_create(window);

  return 0;
}

void app_mainloop() {
  struct mfb_timer *timer = mfb_timer_create();
  struct mfb_timer *tick_timer = mfb_timer_create();
  double total_tick_time = 0.0f;
  double delta = 1.0f;
  double frame_time = delta * 1000;
  double fps = 1.0f / delta;
  diag_listener_add("frame_time", " Frame time: %.3f ms\n", &frame_time);
  diag_listener_add("fps", " FPS: %.0f\n", &fps);

  do {
    int state;
    double dt = mfb_timer_delta(tick_timer);

    mfb_timer_now(timer);

    app_tick(dt);
    app_update();

    uint32_t *window_buffer = renderer_get_buffer(&rn);
    state = mfb_update_ex(window, window_buffer, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (state < 0) {
      window = NULL;
      break;
    }

    delta = mfb_timer_delta(timer);
    frame_time = delta * 1000;
    fps = 1.0f / delta;

    diag_print_info();

  } while (mfb_wait_sync(window));

  diag_exit();
}

static void app_tick(double dt) {
  handle_keypress();
  world_update(dt);
}

static void app_update() {
  renderer_reset_buffer(&rn);
  world_render(&cm);
}

static void handle_keypress() {
  float speed = 4.0f;

  if (kbd_key_is_pressed(kbd, KB_KEY_W)) {
    camera_move_by(&cm, (vec2){0.0f, speed});
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_A)) {
    camera_move_by(&cm, (vec2){-speed, 0.0f});
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_S)) {
    camera_move_by(&cm, (vec2){0.0f, -speed});
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_D)) {
    camera_move_by(&cm, (vec2){speed, 0.0f});
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_Q)) {
    cm.scale *= 1.05f;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_E)) {
    cm.scale *= 0.95f;
  }
  if (kbd_key_is_pressed(kbd, KB_KEY_R)) {
    camera_rotate_by(&cm, 0.05f);
  }
}
