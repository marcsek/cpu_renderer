#include "diagnostics.h"
#include <MiniFB.h>
#include <notstd.h>
#include <stdio.h>
#include <stdlib.h>

void clear_output() {
#ifdef _WIN32
  system("CLS");
#else
  system("clear");
#endif
}

void diag_print_info(struct mfb_timer *tmr) {
  clear_output();
  hash_table *ht = hash_table_create(1024, NULL, NULL);

  double delta = mfb_timer_delta(tmr);
  double frame_time = delta * 1000;
  uint16_t fps = 1.0f / delta;

  printf("-i- DIAGNOSTICS -i-\n\n");
  printf(" Frame time: %.3f ms\n", frame_time);
  printf(" FPS: %d\n", fps);
}

void diag_exit() { printf("\n-!- Window disconnected -!-\n"); }
