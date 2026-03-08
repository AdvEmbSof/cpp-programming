#pragma once

// std
#include <cstdlib>

namespace badly_defined {

class String {
public:
  // constructors
  String(const char* szArray);
  String(size_t size);
  
  // destructor
  ~String();

  // for printing content
  size_t getSize() const { return _size; }
  const char* getContent() const { return _szArray; }
  
private:
  char* _szArray = nullptr;
  size_t _size = 0;
};

} // namespace badly_defined
