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
 * @file string.cpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief String class implementation
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/

#include "string.hpp"

// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// std
#include <cstdlib>
#include <memory>
#include <utility>

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

namespace well_defined {

String::String(const char* szArray) {
  LOG_DBG("Construct from array");
  if (szArray == nullptr) {
    _size       = 0;
    _szArray    = std::make_unique<char[]>(1);
    _szArray[0] = '\0';
    return;
  }
  _size    = strlen(szArray);
  _szArray = std::make_unique<char[]>(_size + 1);
  strncpy(_szArray.get(), szArray, _size);
}

String::String(size_t size) {
  LOG_DBG("Construct from size");
  _size       = size;
  _szArray    = std::make_unique<char[]>(_size + 1);
  _szArray[0] = '\0';
}

// Deep copy constructor — unique_ptr cannot be shared
String::String(const String& other) {
  _size    = other._size;
  _szArray = std::make_unique<char[]>(_size + 1);
  strncpy(_szArray.get(), other._szArray.get(), _size);
}

// Deep copy assignment
String& String::operator=(const String& other) {
  if (this != &other) {
    _size    = other._size;
    _szArray = std::make_unique<char[]>(_size + 1);
    strncpy(_szArray.get(), other._szArray.get(), _size);
  }
  return *this;
}

// Move constructor — transfers ownership, no copy
String::String(String&& other) noexcept
    : _szArray(std::move(other._szArray)), _size(other._size) {
  other._size = 0;
}

// Move assignment — transfers ownership, no copy
String& String::operator=(String&& other) noexcept {
  if (this != &other) {
    _szArray    = std::move(other._szArray);
    _size       = other._size;
    other._size = 0;
  }
  return *this;
}

}  // namespace well_defined
