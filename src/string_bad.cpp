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
 * @file string_bad.cpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief String class implementation (bad implementation)
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/

#include "string_bad.hpp"

// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// std
#include <cstdlib>
#include <memory>

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

namespace badly_defined {

String::String(const char* szArray) {
  LOG_DBG("Construct from const char*");
  _size          = strlen(szArray);
  this->_szArray = new char[_size + 1];  // +1 for null terminator
  strcpy(this->_szArray, szArray);       // NOLINT(runtime/printf)
}

String::String(size_t size) {
  LOG_DBG("Construct from int");
  _size    = size;
  _szArray = new char[_size + 1]{0};  // +1 for null terminator
}

String::~String() {
  if (this->_szArray != nullptr) {
    delete[] this->_szArray;
    this->_szArray = nullptr;
  }
  _size = 0;
}

}  // namespace badly_defined
