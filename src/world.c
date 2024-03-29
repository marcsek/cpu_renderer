#include "world.h"
#include "camera.h"
#include "entity.h"
#include "shapes/shapes.h"
#include <MiniFB.h>
#include <math.h>
#include <notstd.h>

int pos_x = 0;
int pos_y = 0;
uint32_t color = 0xFFFF0000;
entity star[4];

void world_create(struct mfb_window *window) {
  star[0] = entity_create(shapes_star_make(150.0f, 75.0f, 5), color);
  star[1] = entity_create(shapes_star_make(200.0f, 100.0f, 8), color);
  star[2] = entity_create(shapes_star_make(75.0f, 50.0f, 3), color);
  star[3] = entity_create(shapes_star_make(150.0f, 75.0f, 9), color);

  entity_translate_by(&star[0], (vec2){100.0f, 20.0f});
  entity_translate_by(&star[1], (vec2){-150.0f, -80.0f});
  entity_translate_by(&star[2], (vec2){300.0f, -500.0f});
  entity_translate_by(&star[3], (vec2){200.0f, 200.0f});
}

double time = 0.0f;
void world_update(double dt) {
  time += dt;
  for (size_t i = 0; i < 4; i++) {
    entity_rotate_by(&star[i], 0.5f * dt);
    entity_translate_by(&star[i], (vec2){0.0f, 0.0f});
    star[i].scale = 0.5f + fabs(sin(time));
  }
}

void world_render(camera *cm) {
  for (size_t i = 0; i < 4; i++) {
    drawable dr = entity_get_drawable(&star[i]);
    camera_draw(cm, &dr);
    // vector_destroy(dr.model);
  }
}
