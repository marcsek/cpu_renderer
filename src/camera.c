#include "camera.h"
#include "coor_transformer.h"
#include "drawable.h"
#include "vec2.h"

camera camera_create(coordinate_transformer *ct) {
  return (camera){
      .pos = (vec2){0.0f, 0.0f},
      .ct = ct,
      .scale = 1.0f,
  };
}

void camera_move_by(camera *cm, vec2 offset) { vec2_add(&cm->pos, &offset); }

void camera_draw(camera *cm, drawable *dr) {
  drawable_translate(dr, &(vec2){-1.0f * cm->pos.x, -1.0f * cm->pos.y});
  drawable_scale(dr, cm->scale);
  coor_transformer_draw(cm->ct, dr);
}
