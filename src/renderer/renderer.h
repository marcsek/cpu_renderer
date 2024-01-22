#ifndef RENDERER_H
#define RENDERER_H
#include <stdlib.h>

typedef struct _renderer renderer;

renderer *renderer_create(size_t buffer_width, size_t buffer_height);
uint32_t *renderer_get_buffer(renderer *rn);
void renderer_put_pixel(renderer *rn, size_t x, size_t y, uint32_t v);
void renderer_reset_buffer(renderer *rn);
uint32_t *renderer_destroy(renderer *rn);

#endif
