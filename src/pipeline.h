#ifndef PIPELINE_H
#define PIPELINE_H

#include "./shaders/vertex.h"
#include "mat3.h"
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
  mat3 rotation;
  vec3 translation;
  surface tex;
  effect effect;
} pipeline;

typedef struct {
  vertex v0;
  vertex v1;
  vertex v2;
} triangle;

pipeline pipeline_create(renderer *rn, effect ef);
void pipeline_begin_frame(pipeline *p);
void pipeline_draw(pipeline *p, indexed_triangle_list *tri_list);
void pipeline_bind_rotation(pipeline *p, const mat3 *rotation_in);
void pipeline_bind_translation(pipeline *p, const vec3 *translation_in);

#endif
