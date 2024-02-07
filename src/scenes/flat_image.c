#include "../screen_transformer.h"
#include "../shapes/shapes.h"
#include "../surface.h"
#include "scene.h"
#include <MiniFB.h>

static shape c;
static surface s;
screen_transformer st;

static void update(keyboard *kbd, double dt) {}

static void render(renderer *rn) {
  for (unsigned int y = 0; y < s.height; y++) {
    for (unsigned int x = 0; x < s.width; x++) {
      renderer_put_pixel(rn, x, y, surface_get_pixel(&s, x, y));
    }
  }
}

scene scene_flat_image_create() {
  s = surface_from_file("assets/sunf.png");

  return (scene){
      .update = update,
      .render = render,
  };
}
