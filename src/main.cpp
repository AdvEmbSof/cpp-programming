// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// local
#include "c_like_casting.h"
#include "cpp_like_casting.hpp"

LOG_MODULE_REGISTER(programming, CONFIG_APP_LOG_LEVEL);

int main() {
  c_like_casting();
  cpp_like_casting();
  c_vs_cpp_like_casting();
}