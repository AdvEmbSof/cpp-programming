// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// local
#include "c_like_casting.h"
#include "cpp_like_casting.hpp"
#include "classes.hpp"
#include "memory_allocation.hpp"

LOG_MODULE_REGISTER(programming, CONFIG_APP_LOG_LEVEL);

int main() {
  c_like_casting();
  cpp_like_casting();
  // uncommment the next line for running faulty type casting
  // c_vs_cpp_like_casting();
  play_with_classes();
  memory_allocation();
}