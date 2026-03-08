#include "string.hpp"

// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// std
#include <cstdlib>
#include <memory>

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

namespace well_defined {

String::String(const char *szArray) {
  LOG_DBG("Construct from array");
  if (szArray == nullptr) {
    _size = 0;
    _szArray = std::make_unique<char[]>(1);
    _szArray[0] = '\0';
    return;
  }
  _size = strlen(szArray);
  _szArray = std::make_unique<char[]>(_size + 1);
  strcpy(_szArray.get(), szArray);
}

String::String(size_t size) {
  LOG_DBG("Construct from size");
  _size = size;
  _szArray = std::make_unique<char[]>(_size + 1);
  _szArray[0] = '\0';
}

// Deep copy constructor — unique_ptr cannot be shared
String::String(const String &other) {
  _size = other._size;
  _szArray = std::make_unique<char[]>(_size + 1);
  strcpy(_szArray.get(), other._szArray.get());
}

// Deep copy assignment
String& String::operator=(const String &other) {
  if (this != &other) {
    _size = other._size;
    _szArray = std::make_unique<char[]>(_size + 1);
    strcpy(_szArray.get(), other._szArray.get());
  }
  return *this;
}

// Move constructor — transfers ownership, no copy
String::String(String &&other) noexcept : 
_szArray(std::move(other._szArray)), _size(other._size) {
  other._size = 0;
}

// Move assignment — transfers ownership, no copy
String& String::operator=(String &&other) noexcept {
  if (this != &other) {
    _szArray = std::move(other._szArray);
    _size = other._size;
    other._size = 0;
  }
  return *this;
}

} // namespace well_defined