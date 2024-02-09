#ifndef TRIANGLE_RASTERIZER_H
#define TRIANGLE_RASTERIZER_H

#include "pipeline.h"

void triangle_rasterizer_draw(renderer *rn, triangle tr, surface *tex,
                              const pipeline *p);

#endif
