#ifndef SCENE_H
#define SCENE_H

#include "../keyboard.h"
#include "../renderer/renderer.h"

typedef struct {
  void (*update)(keyboard *kbd, double dt);
  void (*render)(renderer *rn);
} scene;

scene scene_cube_skin_create(renderer *rn);
scene scene_color_cube_create(renderer *rn);
scene scene_overlap_cube_create(renderer *rn);
scene pos_cube_create(renderer *rn);
scene scene_flag_plane_create(renderer *rn);

#endif
