#include "app.h"
#include "renderer/renderer.h"
#include "window.h"
#include <MiniFB.h>

static window wnd;
static renderer rn;

static void handle_keypress();
static void app_tick(double dt);
static void app_update();

int app_init() {
  struct mfb_window *mfb_window =
      mfb_open_ex(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, WF_RESIZABLE);

  if (!mfb_window)
    return -1;

  rn = renderer_create(WINDOW_WIDTH, WINDOW_HEIGHT);
  wnd = window_create(mfb_window);

  mfb_set_target_fps(TARGET_FPS);

  return 0;
}

void app_mainloop() {
  struct mfb_timer *timer = mfb_timer_create();
  struct mfb_timer *tick_timer = mfb_timer_create();
  double total_tick_time = 0.0f;
  double delta = 1.0f;
  double frame_time = delta * 1000;
  double fps = 1.0f / delta;

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

  } while (mfb_wait_sync(wnd.mfb_window));
}

static void app_tick(double dt) { handle_keypress(); }

static void app_update() { renderer_reset_buffer(&rn); }

static void handle_keypress() {}
