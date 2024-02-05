#ifndef WINDOW_H
#define WINDOW_H

#include "keyboard.h"
#include <MiniFB.h>
#include <stdlib.h>

typedef struct {
  keyboard *kbd;
  struct mfb_window *mfb_window;
} window;

window window_create(struct mfb_window *mfb_window);
void window_destroy(window *wnd);

#endif
