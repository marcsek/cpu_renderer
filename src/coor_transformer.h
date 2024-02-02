#ifndef COORDINATE_TRANSFORMER_H
#define COORDINATE_TRANSFORMER_H

#include "drawable.h"
#include "renderer/renderer.h"

typedef struct {
  renderer *rn;
} coordinate_transformer;

coordinate_transformer coor_tranformer_create(renderer *rn);
void coor_transformer_draw(coordinate_transformer *ct, drawable *dr);

#endif
