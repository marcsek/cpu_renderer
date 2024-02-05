#ifndef DEBUG_INFO_H
#define DEBUG_INFO_H

#define DEBUG_INFO_MAX_HEADER_COUNT 32

void debug_header_add(const char *template_string, void *value);
void debug_printf(const char *template_string, void *value);
void debug_printf_flush();
void debug_info_output();
void debug_exit();

#endif
