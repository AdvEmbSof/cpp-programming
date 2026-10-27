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

// std
#include <cstdlib>
#include <memory>

// zpp_lib
#include "zpp_include/zpp_log.hpp"

ZPP_LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

namespace badly_defined {

String::String(const char* szArray) 
: _size(strlen(szArray)) {
  ZPP_LOG_DBG("Construct from const char*");
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  this->_szArray = new char[_size + 1];  // +1 for null terminator
  // NOLINTNEXTLINE(clang-analyzer-security.insecureAPI.strcpy)
  strcpy(this->_szArray, szArray);       // NOLINT(runtime/printf)
}

String::String(size_t size) 
: _size(size) {
  ZPP_LOG_DBG("Construct from int");
  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
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
