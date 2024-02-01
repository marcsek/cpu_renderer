#ifndef COORDINATE_TRANSFORMER_H
#define COORDINATE_TRANSFORMER_H

#include "renderer/renderer.h"

typedef struct {
  renderer *rn;
} coordinate_transformer;

coordinate_transformer coor_tranformer_create(renderer *rn);
void coor_transformer_create_closed_polyline(coordinate_transformer *ct,
                                             vec *verts, uint32_t v);

#endif
