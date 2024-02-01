#include "world.h"
#include "coor_transformer.h"
#include "entity.h"
#include "shapes/shapes.h"
#include <notstd.h>

int pos_x = 0;
int pos_y = 0;
entity star;

void world_create() {
  star = entity_create(shapes_star_make(150.0f, 75.0f, 5));
}

void world_update() {
  entity_translate_by(&star, (vec2){0.0f, 0.0f});
  star.scale -= 0.001f;
}

void world_render(coordinate_transformer *ct) {
  coor_transformer_create_closed_polyline(ct, entity_get_polyline(&star),
                                          0xFFFF0000);
}
