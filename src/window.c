#include "window.h"
#include "MiniFB.h"
#include "keyboard.h"
#include <stdio.h>
#include <stdlib.h>

static void keyboard_listener(struct mfb_window *window, mfb_key key,
                              mfb_key_mod mod, bool isPressed) {
  keyboard *kbd = (keyboard *)mfb_get_user_data(window);
  kbd_register_key_press(kbd, key, isPressed);
}

window window_create(struct mfb_window *mfb_window) {
  keyboard *kbd = malloc(sizeof(keyboard));
  *kbd = kbd_create();
  window wnd = (window){
      .mfb_window = mfb_window,
      .kbd = kbd,
  };
  mfb_set_user_data(mfb_window, kbd);
  mfb_set_keyboard_callback(mfb_window, keyboard_listener);

  return wnd;
}

void window_destroy(window *wnd) {
  kbd_destroy(wnd->kbd);
  wnd->mfb_window = NULL;
}
