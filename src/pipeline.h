#ifndef PIPELINE_H
#define PIPELINE_H

#include "renderer/renderer.h"
#include "screen_transformer.h"
#include "shaders/effect.h"
#include "shapes/shapes.h"
#include "surface.h"
#include "z_buffer.h"

typedef struct {
  renderer *rn;
  screen_transformer st;
  z_buffer zb;
  surface tex;
  effect effect;
} pipeline;

pipeline pipeline_create(renderer *rn, effect ef);
void pipeline_begin_frame(pipeline *p);
void pipeline_draw(pipeline *p, indexed_triangle_list *tri_list);

#endif
