#include "app.h"
#include "MiniFB_enums.h"
#include "helpers/debug_info.h"
#include "keyboard.h"
#include "renderer/renderer.h"
#include "scenes/scene.h"
#include "window.h"
#include <MiniFB.h>

static window wnd;
static renderer rn;

// **
#define SCENE_LEN 5
static scene scenes[SCENE_LEN];
size_t cur_scene = 0;
bool did_cycle = false;
// **

static void handle_keypress(double dt);
static void app_tick(double dt);
static void app_render();

int app_init() {
  struct mfb_window *mfb_window =
      mfb_open_ex(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, WF_RESIZABLE);

  if (!mfb_window)
    return -1;

  wnd = window_create(mfb_window);
  rn = renderer_create(WINDOW_WIDTH, WINDOW_HEIGHT);

  mfb_set_target_fps(TARGET_FPS);

  // **
  scenes[0] = scene_flag_plane_create(&rn);
  scenes[1] = scene_color_cube_create(&rn);
  scenes[2] = scene_cube_skin_create(&rn);
  scenes[3] = scene_overlap_cube_create(&rn);
  scenes[4] = pos_cube_create(&rn);
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
    app_render();

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

static void app_tick(double dt) {
  handle_keypress(dt);
  scenes[cur_scene].update(wnd.kbd, dt);
}

static void app_render() {
  renderer_reset_buffer(&rn);
  scenes[cur_scene].render(&rn);
}

static void handle_keypress(double dt) {
  if (kbd_key_is_pressed(wnd.kbd, KB_KEY_TAB) && !did_cycle) {
    cur_scene = (cur_scene + 1) % SCENE_LEN;
    did_cycle = true;
  }
  if (!kbd_key_is_pressed(wnd.kbd, KB_KEY_TAB)) {
    did_cycle = false;
  }
}
