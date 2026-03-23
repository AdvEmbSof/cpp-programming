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
 * @file cpp_like_casting.cpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief Cpp casting function implementation
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/

#include "cpp_like_casting.hpp"

// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// stl
#include <limits>

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

void cpp_like_casting() {
  char i  = 0;
  char j  = 0;
  char* p = &i;
  void* q = p;
  int* pp = static_cast<int*>(q);  // unsafe, but with explicit conversion
  LOG_DBG("%d %d", i, j);
  *pp = -1;  // This overwrites memory starting at &i
  LOG_DBG("%d %d", i, j);
}

class Base {
 protected:
  int32_t _d1 = 0;

 public:
  virtual void m1(int32_t i) {
    _d1 -= i;
    LOG_DBG("Base::m1(): _d1: %d", _d1);
  }
};

class Derived : public Base {
 private:
  int32_t _d2 = 1;

 public:
  void m1(int32_t i) override {
    _d2 += i;
    LOG_DBG("Derived::m1(): _d1: %d _d2: %d", _d1, _d2);
  }
  void m2(int32_t i) {
    _d2 -= i;
    LOG_DBG("Derived::m2(): _d1: %d _d2: %d", _d1, _d2);
  }
};

class Unrelated {
 private:
  int16_t _d3 = 1;

 public:
  void m() {
    _d3++;
    LOG_DBG("Unrelated::m(): _d3: %d", _d3);
  }
};

void c_vs_cpp_like_casting() {
  // Declare one variable of each type.
  Base b;
  Derived d;
  Unrelated u;

  // ###################
  //  USING c-type cast
  //  Primitive types casts
  {
    // Using cast for Base, Derived and Unrelated
    // You can cast almost anything

    // This cast is accepted and correct since Base is a base class to Derived
    // and Naming is a Derived instance
    Base* pb = (Base*)&d;  // NOLINT(readability/casting)
    // it calls m1 from Derived as expected.
    pb->m1(1);

    // This cast is accepted because a Base instance could be a Derived instance.
    // It is correct because pb is effectively pointing to a Derived instance.
    Derived* pd = (Derived*)pb;  // NOLINT(readability/casting)
    // it calls m1 from Derived as expected.
    pd->m1(1);

    // But if pb is now pointing to a Base instance it is accepted.
    // One can then call methods of Derived on a Base instance ! -> undefined behavior.
    pb = &b;
    pd = (Derived*)pb;  // NOLINT(readability/casting)
    // it calls m1 from Base and not Derived...
    pd->m1(1);
    // it calls m2 from Derived but pd is effectively pointing to a Base
    // instance
    pd->m2(1);

    // It is even possible to cast to a type which is totally unrelated to the expression
    // !
    Unrelated* pu = (Unrelated*)&b;  // NOLINT(readability/casting)
    // it calls m() from Unrelated...
    pu->m();

    // The address of any object can be stored in almost any type (as long as it can store
    // the address)
    uint64_t dummy = (uint64_t)&b;  // NOLINT(readability/casting)
    LOG_DBG("Dummy: %lld", dummy);

    // In C++, some c-style casts that would be accepted in C are not accepted
    // Here char cannot store the address to b.
    // The code below does not compile.
    // char ch = (char) &b;
  }

  // ###################
  //  USING static_cast
  //  Primitive types
  //  Conversion with static casts behaves similarly to c-type cast.
  //  It does however reject casting between unrelated types !
  {
    // Using cast for Base, Derived and Unrelated
    // static_cast prevents conversions between unrelated types.
    Base* pb    = static_cast<Base*>(&d);
    Derived* pd = static_cast<Derived*>(pb);
    pb          = &b;
    pd          = static_cast<Derived*>(pb);
    pd->m2(1);
    // this may crash at run time...
    // The code below does not compile
    // Unrelated* pu = static_cast<Unrelated*>(&b);

    // The code below does not compile
    // uint64_t dummy = static_cast<uint64_t>(&b);
  }
}
