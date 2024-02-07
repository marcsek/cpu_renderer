#include "helpers.h"

void hlp_swap(void **v1, void **v2) {
  void *temp = *v1;
  *v1 = *v2;
  *v2 = temp;
}

float minf(float v1, float v2) {
  if (v1 < v2) {
    return v1;
  }
  return v2;
}
