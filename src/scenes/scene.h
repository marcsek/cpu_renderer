#ifndef SCENE_H
#define SCENE_H

#include "../keyboard.h"
#include "../renderer/renderer.h"

typedef struct {
  void (*update)(keyboard *kbd, double dt);
  void (*render)(renderer *rn);
} scene;

scene scene_solid_cube_create(renderer *rn);
scene scene_texture_create(renderer *rn);
scene scene_color_cube_create(renderer *rn);

#endif
