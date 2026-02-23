#include "c_like_casting.h"

// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

void c_like_casting() {
  char i = 0;
  char j = 0;
  char* p = &i;
  void* q = p;
  int* pp = q;    // unsafe, legal C, not C++
  LOG_DBG("%d %d", i, j);
  *pp = -1;       // This overwrites memory starting at &i 
  LOG_DBG("%d %d", i, j);
}

