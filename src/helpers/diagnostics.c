#include "diagnostics.h"
#include "vec.h"
#include <MiniFB.h>
#include <notstd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PRINT_INTERVAL 8.0f

void clear_output() {
#ifdef _WIN32
  system("CLS");
#else
  system("clear");
#endif
}

typedef struct _diag_listener {
  const char *key;
  const char *desc;
  void *value;
} diag_listener;

static vec *listeners = NULL;
static struct mfb_timer *timer = NULL;
static double since_last_print = 1.0f / MAX_PRINT_INTERVAL;

void init() {
  if (listeners == NULL) {
    listeners = vec_create(DIAG_MAX_LISTENER_COUNT);
  }

  if (timer == NULL)
    timer = mfb_timer_create();
}

void diag_listener_add(const char *key, const char *desc, void *value) {
  init();
  diag_listener *lst = (diag_listener *)malloc(sizeof(diag_listener));

  lst->desc = desc;
  lst->value = value;
  vec_push_back(listeners, lst);
}

void diag_print_info() {
  since_last_print += mfb_timer_delta(timer);
  if (since_last_print <= 1.0f / MAX_PRINT_INTERVAL)
    return;
  since_last_print = 0.0f;

  clear_output();

  printf("-i- DIAGNOSTICS -i-\n\n");

  diag_listener **data = (diag_listener **)vec_get_data(listeners);
  for (size_t i = 0; i < vec_get_size(listeners); i++) {
    double d = *((double *)data[i]->value);
    printf(data[i]->desc, d);
  }
}

void diag_exit() {
  printf("\n-!- Window disconnected -!-\n");
  // Este treba cleanup
  vec_destroy(listeners);
}
