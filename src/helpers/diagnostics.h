#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H
#include <MiniFB.h>

#define DIAG_MAX_LISTENER_COUNT 32

void diag_listener_add(const char *key, const char *desc, void *value);
void diag_print_info();
void diag_exit();

#endif
