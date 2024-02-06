#ifndef SCENE_H
#define SCENE_H

#include "../keyboard.h"
#include "../renderer/renderer.h"

typedef struct {
  void (*update)(keyboard *kbd, double dt);
  void (*render)(renderer *rn);
} scene;

scene scene_solid_cube_create();
scene scene_cube_order_create();

#endif
