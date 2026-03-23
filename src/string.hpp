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
 * @file string.hpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief String class declaration
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/
#pragma once

// std
#include <cstdlib>
#include <memory>

namespace well_defined {

class String {
 public:
  // constructors
  explicit String(const char* szArray);
  explicit String(size_t size);
  ~String() = default;

  // copy constructor
  String(const String& other);
  // assignment operator
  String& operator=(const String& other);
  // Move constructor — transfers ownership, no copy
  String(String&& other) noexcept;
  // Move assignment — transfers ownership, no copy
  String& operator=(String&& other) noexcept;

  // for printing content
  size_t getSize() const { return _size; }
  const char* getContent() const { return _szArray ? _szArray.get() : ""; }

 private:
  std::unique_ptr<char[]> _szArray;
  size_t _size = 0;
};

}  // namespace well_defined
