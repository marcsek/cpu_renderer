#include "debug_info.h"
#include "MiniFB.h"
#include "vector.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUTPUT_INTERVAL 8.0f
#define MAX_PRINTF_STRING_LEN 128 * sizeof(char)

void clear_output() {
#ifdef _WIN32
  system("CLS");
#else
  system("clear");
#endif
}

typedef struct {
  const char *template_string;
  void *value;
} debug_info_value;

static vector *header_info;
static vector *printf_info;
static struct mfb_timer *output_timer;
static double since_last_output = 0.0f;

static void init() {
  if (header_info == NULL) {
    header_info = vector_create(DEBUG_INFO_MAX_HEADER_COUNT);
  }
  if (printf_info == NULL) {
    printf_info = vector_create_cf(64, free);
  }
  if (output_timer == NULL) {
    output_timer = mfb_timer_create();
  }
}

void debug_info_output() {
  init();
  since_last_output += mfb_timer_delta(output_timer);
  if (since_last_output < 1.0f / OUTPUT_INTERVAL)
    return;
  since_last_output = 0.0f;

  clear_output();

  printf("| - DEBUG INFO - \n");

  debug_info_value **header_vals =
      (debug_info_value **)vector_get_data(header_info);

  for (size_t i = 0; i < vector_get_size(header_info); i++) {
    if (i != vector_get_size(header_info) - 1) {
      printf("|\\\n| \\_");
    } else {
      printf(" \\\n  \\_");
    }

    printf(header_vals[i]->template_string, *(double *)header_vals[i]->value);
    printf("\n");
  }

  printf("\n - LOG MESSAGES - \n\n");

  char **printf_vals = (char **)vector_get_data(printf_info);

  for (size_t i = 0; i < vector_get_size(printf_info); i++) {
    printf("%s\n", printf_vals[i]);
  }
}

void debug_header_add(const char *template_string, void *value) {
  init();
  debug_info_value *v = malloc(sizeof(debug_info_value));
  *v = (debug_info_value){.value = value, .template_string = template_string};
  vector_push_back(header_info, v);
}

void debug_printf(const char *template_string, ...) {
  init();

  va_list args;
  va_start(args, template_string);

  char *str = malloc(MAX_PRINTF_STRING_LEN + 1);
  vsnprintf(str, MAX_PRINTF_STRING_LEN, template_string, args);
  vector_push_back(printf_info, str);

  va_end(args);
}

void debug_printf_flush() {
  init();
  vector_destroy(printf_info);
  printf_info = NULL;
}

void debug_exit() {}
