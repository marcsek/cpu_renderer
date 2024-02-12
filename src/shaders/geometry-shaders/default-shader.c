#include "../effect.h"
#include "essentials.h"

typedef struct {
} default_shader_data;

static triangle process(void *data, const vertex *in1, const vertex *in2,
                        const vertex *in3, unsigned int index) {
  UNUSED(data);
  UNUSED(index);

  vertex v0_c = vertex_copy(in1);
  vertex v1_c = vertex_copy(in2);
  vertex v2_c = vertex_copy(in3);

  return (triangle){.v0 = v0_c, .v1 = v1_c, .v2 = v2_c};
}

geometry_shader default_geometry_create() {
  default_shader_data *d = malloc(sizeof(default_shader_data));
  return (geometry_shader){
      .process = process,
      .shader_data = d,
  };
}
