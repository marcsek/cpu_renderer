#include "camera.h"
#include "coor_transformer.h"
#include "drawable.h"
#include "vec2.h"

camera camera_create(coordinate_transformer *ct) {
  return (camera){
      .pos = (vec2){0.0f, 0.0f},
      .ct = ct,
      .scale = 1.0f,
      .angle = 0.0f,
  };
}

void camera_move_by(camera *cm, vec2 offset) { vec2_add(&cm->pos, &offset); }
void camera_rotate_by(camera *cm, float angle_in) { cm->angle += angle_in; }

void camera_draw(camera *cm, drawable *dr) {
  vec2 translation = (vec2){-1.0f * cm->pos.x, -1.0f * cm->pos.y};
  // account for camera rotation
  vec2_rotate(&translation, -cm->angle);
  //
  drawable_translate(dr, &translation);
  drawable_scale(dr, cm->scale);
  drawable_rotate(dr, cm->angle);
  coor_transformer_draw(cm->ct, dr);
}
