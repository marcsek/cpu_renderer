#include "../effect.h"
#include "../vertex.h"
#include <stdint.h>

static uint32_t create_pixel(void *data, const vertex *inv) {
  UNUSED(data);
  UNUSED(inv);

  return 0xFFFF0000;
}

pixel_shader default_pixel_create() {
  return (pixel_shader){
      .create_pixel = create_pixel,
  };
}
