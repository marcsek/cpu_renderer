#include "keyboard.h"
#include <stdlib.h>

#define TOTAL_KEY_CODES 512

keyboard kbd_create() {
  keyboard kbd = (keyboard){
      .keystate = malloc(sizeof(bool) * TOTAL_KEY_CODES),
  };
  return kbd;
}

bool kbd_key_is_pressed(keyboard *kbd, int key_code) {
  if (key_code != -1 && key_code < TOTAL_KEY_CODES) {
    return kbd->keystate[key_code];
  }
  return false;
}

void kbd_register_key_press(keyboard *kbd, int key_code, bool state) {
  if (key_code != -1 && key_code < TOTAL_KEY_CODES) {
    kbd->keystate[key_code] = state;
  }
}

void kbd_destroy(keyboard *kbd) {
  free(kbd->keystate);
  free(kbd);
}
