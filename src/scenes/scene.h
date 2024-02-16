#ifndef SCENE_H
#define SCENE_H

#include "../keyboard.h"
#include "../renderer/renderer.h"

typedef struct {
  void (*update)(keyboard *kbd, double dt);
  void (*render)();
} scene;

scene scene_cube_skin_create(renderer *rn);
scene scene_color_cube_create(renderer *rn);
scene scene_solid_face_create(renderer *rn);
scene scene_overlap_cube_create(renderer *rn);
scene pos_cube_create(renderer *rn);
scene scene_flag_plane_create(renderer *rn);
scene scene_bunny_create(renderer *rn);
scene scene_suz_create(renderer *rn);
scene test_cube_create(renderer *rn);

#endif
