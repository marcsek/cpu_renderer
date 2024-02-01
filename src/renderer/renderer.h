#ifndef RENDERER_H
#define RENDERER_H
#include <notstd.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct _renderer {
  uint buffer_width;
  uint buffer_height;
  size_t buffer_size;
  uint32_t *_buffer;
} renderer;

renderer renderer_create(size_t buffer_width, size_t buffer_height);
uint32_t *renderer_get_buffer(renderer *rn);
void renderer_put_pixel(renderer *rn, uint x, uint y, uint32_t v);
void renderer_reset_buffer(renderer *rn);
uint32_t *renderer_destroy(renderer *rn);

void renderer_create_line(renderer *rn, int x1, int y1, int x2, int y2,
                          uint32_t v);
void renderer_create_closed_polyline(renderer *rn, vector *verts, uint32_t v);

#endif
