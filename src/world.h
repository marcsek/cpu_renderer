#ifndef WORLD_H
#define WORLD_H

#include "coor_transformer.h"

typedef struct {
} world;

void world_create();
void world_update();
void world_render(coordinate_transformer *ct);

#endif
