#include "../surface.h"
#include "scene.h"
#include <MiniFB.h>

surface sf;

static void update(keyboard *kbd, double dt) {}

static void render(renderer *rn) {
  for (size_t y = 0; y < sf.height; y++) {
    for (size_t x = 0; x < sf.width; x++) {
      renderer_put_pixel(rn, x, y, surface_get_pixel(&sf, x, y));
    }
  }
}

scene scene_texture_create(renderer *rn) {
  sf = surface_from_file("assets/sunf.png");

  return (scene){
      .update = update,
      .render = render,
  };
}
