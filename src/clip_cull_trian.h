#ifndef CLIP_CULL_TRIAN
#define CLIP_CULL_TRIAN

#include "pipeline.h"
#include "triangle.h"

typedef void (*pptv)(pipeline *p, triangle tr);

void clip_cull_triangle(pipeline *p, triangle *t, pptv pptv_func);

#endif
