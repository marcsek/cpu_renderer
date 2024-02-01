#include "shapes.h"
#include <math.h>
#include <stdlib.h>

vector *shapes_star_make(float outerRadius, float innerRadius, int nFlares) {
  vector *star = vector_create(nFlares * 2);
  const float dTheta = 2.0f * 3.14159f / (float)(nFlares * 2);

  for (int i = 0; i < nFlares * 2; i++) {
    float rad = innerRadius;
    if (i % 2 == 0)
      rad = outerRadius;

    vec2 *v = malloc(sizeof(vec2));
    v->x = rad * cosf(((float)i) * dTheta);
    v->y = rad * sinf(((float)i) * dTheta);

    vector_push_back(star, v);
  }

  return star;
}
