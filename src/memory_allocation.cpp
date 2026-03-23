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
#include <zephyr/logging/log.h>

// std
#include <cstdlib>
#include <memory>

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

class MyClass {
 public:
  // constructor
  MyClass() {
    _a = 2;
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
    __ASSERT(size == sizeof(MyClassWithSlab), "Incorrect size %zu", size);
    void* block_ptr = nullptr;
    int ret         = k_mem_slab_alloc(&MY_CLASS_SLAB, &block_ptr, K_MSEC(0));
    __ASSERT(ret == 0, "MyClassWithPool slab exhausted");
    LOG_DBG("Number of used slots: %d", k_mem_slab_num_used_get(&MY_CLASS_SLAB));
    return block_ptr;
  }

  void operator delete(void* p) {
    k_mem_slab_free(&MY_CLASS_SLAB, p);
    LOG_DBG("Number of used slots: %d", k_mem_slab_num_used_get(&MY_CLASS_SLAB));
  }

  // Prevent accidental array allocation falling back to global heap
  void* operator new[](size_t)  = delete;
  void operator delete[](void*) = delete;

  uint32_t _a = 0;
};

// Defined after class — sizeof() and alignof() are now available
static constexpr uint8_t NBR_OF_BLOCKS = 10;
K_MEM_SLAB_DEFINE(MY_CLASS_SLAB,
                  sizeof(MyClassWithSlab),
                  NBR_OF_BLOCKS,
                  alignof(MyClassWithSlab));

void memory_allocation() {
  // Object not constructed!, cast required, size to be specified
  MyClass* pInstance1 = (MyClass*)malloc(sizeof(MyClass));  // NOLINT(readability/casting)
  // Uncomment the next line (compiler warning because pInstance1 is not constructed)
  // printk("pInstance1->_a: %d\n", pInstance1->_a);

  // malloc returns nullptr when memory cannot be allocated
  if (pInstance1 == nullptr) {
    printk("Cannot allocate memory\n");
  }

  // destructor not called!
  free(pInstance1);
  pInstance1 = nullptr;

  // Constructor is called, no cast required, no size specified
  // When memory cannot be allocated, new generates a exception or
  // returns nullptr, depending on implementation.
  // On zephyr rtos (and embedded systems), exceptions are usually not used.
  // try {
  MyClass* pInstance2 = new MyClass();
  printk("pInstance2->_a: %d\n", pInstance2->_a);
  //}
  // catch (const std::bad_alloc &e) {
  //
  //}

  if (pInstance2 == nullptr) {
    printk("Cannot allocate memory\n");
  }

  {
    MyClassWithSlab* pInstanceWithSlab1 = new MyClassWithSlab();
    printk("pInstanceWithPool->_a: %d\n", pInstanceWithSlab1->_a);
    MyClassWithSlab* pInstanceWithSlab2 = new MyClassWithSlab();
    printk("pInstanceWithPool->_a: %d\n", pInstanceWithSlab2->_a);
    delete pInstanceWithSlab1;
    pInstanceWithSlab1 = nullptr;
    delete pInstanceWithSlab2;
    pInstanceWithSlab2 = nullptr;
  }

  // Proper allocation with smart pointers
  {
    auto pInstanceWithSlab = std::make_unique<MyClassWithSlab>();
    printk("pInstanceWithSlab->_a: %d\n", pInstanceWithSlab->_a);
    LOG_DBG("Address of instance is %p", pInstanceWithSlab.get());

    for (uint8_t i = 0; i < NBR_OF_BLOCKS / 2; i++) {
      auto pOtherInstanceWithSlab = std::make_unique<MyClassWithSlab>();
      printk("i: %d, pOtherInstanceWithSlab->_a: %d\n", i, pOtherInstanceWithSlab->_a);
      // The address should be the same at each iteration, but different from
      // pInstanceWithSlab
      LOG_DBG("i: %d, Address of instance is %p", i, pOtherInstanceWithSlab.get());
    }
  }
}
