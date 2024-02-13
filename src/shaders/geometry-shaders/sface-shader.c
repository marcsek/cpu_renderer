#include "../effect.h"
#include "essentials.h"
#include <stdint.h>

#define get_sd(var, type)                                                      \
  { .pos = (var)->pos, .color = ((type *)(var)->sd) }

typedef struct {
  uint32_t *colors;
} sface_shader_data;

static triangle process(void *data, const vertex *in1, const vertex *in2,
                        const vertex *in3, unsigned int index) {
  UNUSED(index);

  sface_shader_data *sd = (sface_shader_data *)data;

  uint32_t *c0 = malloc(sizeof(uint32_t));
  uint32_t *c1 = malloc(sizeof(uint32_t));
  uint32_t *c2 = malloc(sizeof(uint32_t));
  *c0 = sd->colors[index / 2];
  *c1 = sd->colors[index / 2];
  *c2 = sd->colors[index / 2];

  vertex v0_c = scolor_vertex_create(in1->pos, c0);
  vertex v1_c = scolor_vertex_create(in2->pos, c1);
  vertex v2_c = scolor_vertex_create(in3->pos, c2);

  return (triangle){.v0 = v0_c, .v1 = v1_c, .v2 = v2_c};
}

void sface_geo_bind_colors(void *data, uint32_t *colors) {
  sface_shader_data *sd = (sface_shader_data *)data;

  sd->colors = colors;
}

geometry_shader sface_geometry_create() {
  sface_shader_data *d = malloc(sizeof(sface_shader_data));

  return (geometry_shader){
      .process = process,
      .shader_data = d,
  };
}
