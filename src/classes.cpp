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
 * @file classes.cpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief play_with_classes function implementation
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/

#include "classes.hpp"

// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// std
#include <cstdlib>

// local
#include "string.hpp"
#include "string_bad.hpp"

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

void play_with_classes() {
  {
    // Create a String instance on the heap, calls the String(const char*) constructor.
    badly_defined::String* s1 = new badly_defined::String("s1");
    LOG_DBG("badly_defined::s1 is %s (length %d)", s1->getContent(), s1->getSize());
    // Create a String instance on the stack, calls the String(const char*) constructor.
    badly_defined::String s2("s2");
    LOG_DBG("badly_defined::s2 is %s (length %d)", s2.getContent(), s2.getSize());
    // Create a String instance on the heap, calls the String(int size) constructor
    badly_defined::String* s3 = new badly_defined::String(10);
    LOG_DBG("badly_defined::s3 is %s (length %d)", s3->getContent(), s3->getSize());
    // Create a string instance on the stack, also calls the String(const char*)
    // constructor!
    badly_defined::String s4 = "hello";
    LOG_DBG("badly_defined::s5 is %s (length %d)", s4.getContent(), s4.getSize());
    // Create a string instance on the stack, also calls the String(int size) constructor!
    badly_defined::String s5 = 10;
    LOG_DBG("badly_defined::s5 is %s (length %d)", s5.getContent(), s5.getSize());

    // Uncomment the following line to get a fatal error.
    // Make sure that you understand the reason for the fatal error.
    // badly_defined::String s6 = '1234';
  }

  {
    badly_defined::String s1("s1");
    {
      // uncomment the following line to get a fatal assertion.
      // Make sure that you understand the reason for the fatal error.
      // badly_defined::String s2 = s1;
    }
  }

  {
    // Create a String instance on the heap, calls the String(const char*) constructor.
    well_defined::String* s1 = new well_defined::String("s1");
    LOG_DBG("well_defined::s1 is %s (length %d)", s1->getContent(), s1->getSize());
    // Create a String instance on the stack, calls the String(const char*) constructor.
    well_defined::String s2("s2");
    LOG_DBG("well_defined::s2 is %s (length %d)", s2.getContent(), s2.getSize());
    // Create a String instance on the heap, calls the String(int size) constructor
    well_defined::String* s3 = new well_defined::String(10);
    LOG_DBG("well_defined::s3 is %s (length %d)", s3->getContent(), s3->getSize());
    // The following construction is forbidden because the constructor is now declared
    // explicit well_defined::String s4 = 10; LOG_DBG("s4 is %s (length %d)",
    // s4.getContent(), s4.getSize());

    // The following construction is forbidden because the constructor is now declared
    // explicit well_defined::String s5 = '1234';
  }

  {
    well_defined::String s1("s1");
    {
      // this calls the copy constructor
      well_defined::String s2 = s1;
      // this calls the assignment operator
      s1 = s2;
    }
  }
}
