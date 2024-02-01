#include "app.h"
#include "coor_transformer.h"
#include "helpers/diagnostics.h"
#include "renderer/renderer.h"
#include "world.h"
#include <MiniFB.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

static struct mfb_window *window;
static renderer rn;
static world wrld;
static coordinate_transformer ct;

void app_tick(double dt);
void app_update();

// void mouse_move_callback(struct mfb_window *w, int x, int y) {
//   pos_x = x;
//   pos_y = y;
// }

int app_init() {
  window = mfb_open_ex(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, WF_RESIZABLE);

  if (!window)
    return -1;

  rn = renderer_create(WINDOW_WIDTH, WINDOW_HEIGHT);

  ct = coor_tranformer_create(&rn);

  mfb_set_target_fps(TARGET_FPS);

  world_create();

  // mfb_set_mouse_move_callback(window, mouse_move_callback);
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

void app_tick(double dt) { world_update(); }

void app_update() {
  renderer_reset_buffer(&rn);
  world_render(&ct);

  // renderer_create_line(rn, 300, 400, pos_x, pos_y);
}
