#ifndef CAMERA_H
#define CAMERA_H

#include "coor_transformer.h"
#include <notstd.h>

typedef struct {
  vec2 pos;
  coordinate_transformer *ct;
  float scale;
} camera;

camera camera_create(coordinate_transformer *ct);
void camera_move_by(camera *cm, vec2 offset);
void camera_draw(camera *cm, drawable *dr);
#endif
