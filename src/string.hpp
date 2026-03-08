#pragma once

// std
#include <cstdlib>
#include <memory>

namespace well_defined {

class String {
public:
    // constructors
    explicit String(const char *szArray);
    explicit String(size_t size);
    ~String() = default;

    // copy constructor
    String(const String &other);
    // assignment operator
    String &operator=(const String &other);
    // Move constructor — transfers ownership, no copy
    String(String &&other) noexcept;
    // Move assignment — transfers ownership, no copy
    String &operator=(String &&other) noexcept;

    // for printing content
    size_t getSize() const { return _size; }
    const char *getContent() const { return _szArray ? _szArray.get() : ""; }

private:
    std::unique_ptr<char[]> _szArray;
    size_t _size = 0;
};

} // namespace well_defined