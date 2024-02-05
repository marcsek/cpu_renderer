#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
typedef struct {
  bool *keystate;
} keyboard;

keyboard kbd_create();
bool kbd_key_is_pressed(keyboard *kbd, int key_code);
void kbd_register_key_press(keyboard *kbd, int key_code, bool state);
void kbd_destroy(keyboard *kbd);

#endif
