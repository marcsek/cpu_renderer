#include "app.h"
#include <MiniFB.h>
#include <stdio.h>

int main() {
  int status = app_init();

  if (status < 0) {
    fprintf(stderr, "Couldn't initialize application.");
    return 1;
  }
  app_mainloop();
}
