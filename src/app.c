#include "app.h"
#include "helpers/diagnostics.h"
#include "renderer/renderer.h"
#include <MiniFB.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

static struct mfb_window *window;
static renderer *rn;
float pos_x = 0;

void app_tick(double dt);
void app_update();
void update_tick(struct mfb_timer *tmr, double *total_time);

int app_init() {
  window = mfb_open_ex(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, WF_RESIZABLE);

  if (!window)
    return -1;

  rn = renderer_create(WINDOW_WIDTH, WINDOW_HEIGHT);

  mfb_set_target_fps(TARGET_FPS);

  return 0;
}

void app_mainloop() {
  struct mfb_timer *timer = mfb_timer_create();
  struct mfb_timer *tick_timer = mfb_timer_create();
  double total_tick_time = 0.0f;

  do {
    int state;
    double dt = mfb_timer_delta(tick_timer);

    mfb_timer_now(timer);

    app_tick(dt);
    app_update();

    uint32_t *window_buffer = renderer_get_buffer(rn);
    state = mfb_update_ex(window, window_buffer, WINDOW_WIDTH, WINDOW_HEIGHT);

    if (state < 0) {
      window = NULL;
      break;
    }

    diag_print_info(timer);

  } while (mfb_wait_sync(window));

  diag_exit();
}

void app_tick(double dt) { pos_x += 100 * dt; }

void app_update() {
  renderer_reset_buffer(rn);
  renderer_put_pixel(rn, MIN(MAX(0, pos_x), WINDOW_WIDTH - 1), 300, 0xFFFF0000);
}
