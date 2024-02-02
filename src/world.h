#ifndef WORLD_H
#define WORLD_H

#include "MiniFB_enums.h"
#include "camera.h"
typedef struct {
} world;

void world_create(struct mfb_window *window);
void world_update();
void world_render(camera *cm);

#endif
