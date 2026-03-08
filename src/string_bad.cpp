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
  _size = strlen(szArray);
  this->_szArray = new char[_size + 1]; // +1 for null terminator
  strcpy(this->_szArray, szArray);
}

String::String(size_t size) {
  LOG_DBG("Construct from int");
  _size = size;
  _szArray = new char[_size + 1]{0}; // +1 for null terminator
}
  
String::~String() {
  if (this->_szArray != nullptr) {
    delete[] this->_szArray;
    this->_szArray = nullptr;
  }
  _size = 0;
}

} // namespace badly_defined