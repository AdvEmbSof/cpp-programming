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
 * @brief Memory allocation function implementation
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/

#include "memory_allocation.hpp"

// zephyr
#include <zephyr/kernel.h>

// std
#include <cstdlib>
#include <memory>

// zpp_lib
#include "zpp_include/zpp_assert.hpp"
#include "zpp_include/zpp_log.hpp"

ZPP_LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

// NOLINTBEGIN

class MyClass {
 public:
  // constructor
  MyClass() : _a(2), _p(nullptr) {
    _p = (uint8_t*)malloc(sizeof(uint8_t) * SIZE);  // NOLINT(readability/casting)
  }

  ~MyClass() {
    if (_p != nullptr) {
      free(_p);
    }
  }

  uint8_t _a                    = 0;
  uint8_t* _p                   = nullptr;
  static constexpr uint8_t SIZE = 10;
};

// Forward declare — slab defined after class
extern struct k_mem_slab MY_CLASS_SLAB;

class MyClassWithSlab {
 public:
  MyClassWithSlab() : _a(2) {}
  ~MyClassWithSlab() {}

  void* operator new(size_t size) {
    ZPP_ASSERT(size == sizeof(MyClassWithSlab), "Incorrect size %zu", size);
    void* block_ptr = nullptr;
    ZPP_LOG_DBG("Allocating memory from slab");
    int ret         = k_mem_slab_alloc(&MY_CLASS_SLAB, &block_ptr, K_MSEC(0));
    ZPP_ASSERT(ret == 0, "MyClassWithPool slab exhausted");
    ZPP_LOG_DBG("Number of used slots: %d", k_mem_slab_num_used_get(&MY_CLASS_SLAB));
    return block_ptr;
  }

  void operator delete(void* p) {
    k_mem_slab_free(&MY_CLASS_SLAB, p);
    ZPP_LOG_DBG("Number of used slots: %d", k_mem_slab_num_used_get(&MY_CLASS_SLAB));
  }

  // Prevent accidental array allocation falling back to global heap
  void* operator new[](size_t)  = delete;
  void operator delete[](void*) = delete;

  uint32_t _a = 0;
};

// Defined after class — sizeof() and alignof() are now available
static constexpr uint8_t kNbrOfBlocks = 10;
K_MEM_SLAB_DEFINE(MY_CLASS_SLAB,
                  sizeof(MyClassWithSlab),
                  kNbrOfBlocks,
                  alignof(MyClassWithSlab));

void memory_allocation() {
  // Object not constructed!, cast required, size to be specified
  auto* p_instance1 = static_cast<MyClass*>(malloc(sizeof(MyClass)));  // NOLINT(readability/casting)
  // Uncomment the next line (compiler warning because pInstance1 is not constructed)
  // printk("pInstance1->_a: %d\n", pInstance1->_a);

  // malloc returns nullptr when memory cannot be allocated
  if (p_instance1 == nullptr) {
    printk("Cannot allocate memory\n");
  }

  // destructor not called!
  free(p_instance1);
  p_instance1 = nullptr;

  // Constructor is called, no cast required, no size specified
  // When memory cannot be allocated, new generates a exception or
  // returns nullptr, depending on implementation.
  // On zephyr rtos (and embedded systems), exceptions are usually not used.
  // try {
  auto* p_instance2 = new MyClass();
  printk("p_instance2->_a: %d\n", p_instance2->_a);
  //}
  // catch (const std::bad_alloc &e) {
  //
  //}

  if (p_instance2 == nullptr) {
    printk("Cannot allocate memory\n");
  }

  {
    auto* p_instance_with_slab1 = new MyClassWithSlab();
    printk("p_instance_with_slab1->_a: %d\n", p_instance_with_slab1->_a);
    auto* p_instance_with_slab2 = new MyClassWithSlab();
    printk("p_instance_with_slab2->_a: %d\n", p_instance_with_slab2->_a);
    delete p_instance_with_slab1;
    p_instance_with_slab1 = nullptr;
    delete p_instance_with_slab2;
    p_instance_with_slab2 = nullptr;
  }

  // Proper allocation with smart pointers
  {
    auto p_instance_with_slab = std::make_unique<MyClassWithSlab>();
    printk("p_instance_with_slab->_a: %d\n", p_instance_with_slab->_a);
    ZPP_LOG_DBG("Address of instance is %p", p_instance_with_slab.get());

    for (uint8_t i = 0; i < kNbrOfBlocks / 2; i++) {
      auto p_other_instance_with_slab = std::make_unique<MyClassWithSlab>();
      printk("i: %d, p_other_instance_with_slab->_a: %d\n", i, p_other_instance_with_slab->_a);
      // The address should be the same at each iteration, but different from
      // p_instance_with_slab
      ZPP_LOG_DBG("i: %d, Address of instance is %p", i, p_other_instance_with_slab.get());
    }
  }
}

// NOLINTEND