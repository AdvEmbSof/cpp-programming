// Copyright 2025 Haute école d'ingénierie et d'architecture de Fribourg
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/****************************************************************************
 * @file main.cpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief Main program for demonstrating C/C++ programming important concepts
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/
// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// local
#include "c_like_casting.h"
#include "callback_fp.h"
#include "callback_lambda.hpp"
#include "classes.hpp"
#include "cpp_like_casting.hpp"
#include "memory_allocation.hpp"

LOG_MODULE_REGISTER(programming, CONFIG_APP_LOG_LEVEL);

int main() {
  // c_like_casting();
  // cpp_like_casting();
  // uncommment the next line for running faulty type casting
  // c_vs_cpp_like_casting();
  // play_with_classes();
  // memory_allocation();
  callback_fp();
  callback_lambda();
}
