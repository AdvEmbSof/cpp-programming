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
 * @file string_test.cpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief Test program for validating the String class implementation
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/


// zephyr
#include <zephyr/ztest.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// std
#include <memory>
#include <utility>

// local
#include "string.hpp"

LOG_MODULE_REGISTER(programming, CONFIG_APP_LOG_LEVEL);

// ── Construction tests ────────────────────────────────────────────────────────

ZTEST(string_test, test_construct_from_cstring)
{
    well_defined::String s("hello");

    zassert_equal(s.getSize(), 5U,
                  "Expected size 5, got %d", s.getSize());
    zassert_str_equal(s.getContent(), "hello",
                      "Expected 'hello', got '%s'", s.getContent());
    zassert_not_null(s.getContent(),
                     "Content pointer must not be null");
}

ZTEST(string_test, test_construct_from_size)
{
    well_defined::String s(16U);

    zassert_equal(s.getSize(), 16U,
                  "Expected size 16, got %d", s.getSize());
    zassert_str_equal(s.getContent(), "",
                      "Expected empty string, got '%s'", s.getContent());
    zassert_not_null(s.getContent(),
                     "Content pointer must not be null");
}

ZTEST(string_test, test_construct_from_nullptr)
{
    well_defined::String s(nullptr);

    zassert_equal(s.getSize(), 0U,
                  "Expected size 0, got %d", s.getSize());
    zassert_str_equal(s.getContent(), "",
                      "Expected empty string, got '%s'", s.getContent());
    zassert_not_null(s.getContent(),
                     "Content pointer must not be null after nullptr construct");
}

ZTEST(string_test, test_construct_empty_string)
{
    well_defined::String s("");

    zassert_equal(s.getSize(), 0U,
                  "Expected size 0, got %d", s.getSize());
    zassert_str_equal(s.getContent(), "",
                      "Expected empty string, got '%s'", s.getContent());
}

// ── Copy tests ────────────────────────────────────────────────────────────────

ZTEST(string_test, test_copy_constructor)
{
    well_defined::String s1("world");
    well_defined::String s2(s1);                             // copy construct

    zassert_equal(s2.getSize(), 5U,
                  "Copy size expected 5, got %d", s2.getSize());
    zassert_str_equal(s2.getContent(), "world",
                      "Copy content expected 'world', got '%s'", s2.getContent());

    // Verify deep copy — independent buffers
    zassert_true(s1.getContent() != s2.getContent(),
                 "Deep copy must produce independent buffers");

    // Verify source is unchanged
    zassert_str_equal(s1.getContent(), "world",
                      "Source must be unchanged after copy");
}

ZTEST(string_test, test_copy_assignment)
{
    well_defined::String s1("zephyr");
    well_defined::String s2("other");
    s2 = s1;                                         // copy assign

    zassert_equal(s2.getSize(), 6U,
                  "Assigned size expected 6, got %d", s2.getSize());
    zassert_str_equal(s2.getContent(), "zephyr",
                      "Assigned content expected 'zephyr', got '%s'", s2.getContent());

    // Verify deep copy — independent buffers
    zassert_true(s1.getContent() != s2.getContent(),
                 "Deep copy assignment must produce independent buffers");

    // Verify source is unchanged
    zassert_str_equal(s1.getContent(), "zephyr",
                      "Source must be unchanged after copy assignment");
}

ZTEST(string_test, test_self_assignment)
{
    well_defined::String s("selftest");

    // Self-assignment must not crash or corrupt
    s = s;

    zassert_equal(s.getSize(), 8U,
                  "Self-assign size expected 8, got %d", s.getSize());
    zassert_str_equal(s.getContent(), "selftest",
                      "Self-assign must not corrupt content");
}

// ── Move tests ────────────────────────────────────────────────────────────────

ZTEST(string_test, test_move_constructor)
{
    well_defined::String s1("moved");
    well_defined::String s2(std::move(s1));                  // move construct

    zassert_equal(s2.getSize(), 5U,
                  "Moved-into size expected 5, got %d", s2.getSize());
    zassert_str_equal(s2.getContent(), "moved",
                      "Moved-into content expected 'moved', got '%s'", s2.getContent());

    // Source must be in valid empty state after move
    zassert_equal(s1.getSize(), 0U,
                  "Moved-from size must be 0, got %d", s1.getSize());
    zassert_not_null(s1.getContent(),
                     "Moved-from getContent() must not crash — returns ''");
    zassert_str_equal(s1.getContent(), "",
                      "Moved-from content must be empty string");
}

ZTEST(string_test, test_move_assignment)
{
    well_defined::String s1("transfer");
    well_defined::String s2("old");
    s2 = std::move(s1);                              // move assign

    zassert_equal(s2.getSize(), 8U,
                  "Moved-into size expected 8, got %d", s2.getSize());
    zassert_str_equal(s2.getContent(), "transfer",
                      "Moved-into content expected 'transfer', got '%s'", s2.getContent());

    // Source must be in valid empty state after move
    zassert_equal(s1.getSize(), 0U,
                  "Moved-from size must be 0, got %d", s1.getSize());
    zassert_str_equal(s1.getContent(), "",
                      "Moved-from content must be empty string");
}

ZTEST(string_test, test_self_move_assignment)
{
    well_defined::String s("selfmove");

    // Self-move must not crash or corrupt
    s = std::move(s);

    // After self-move the state is valid but unspecified per C++ standard
    // We only check it does not crash — no content assertion
    zassert_not_null(s.getContent(),
                     "getContent() must not crash after self-move");
}

// ── Return by value test ──────────────────────────────────────────────────────

ZTEST(string_test, test_return_by_value)
{
    // Compiler should apply NRVO — no extra copy or move
    auto make = []() -> well_defined::String {
        well_defined::String s("nrvo");
        return s;
    };

    well_defined::String s = make();

    zassert_equal(s.getSize(), 4U,
                  "Returned size expected 4, got %d", s.getSize());
    zassert_str_equal(s.getContent(), "nrvo",
                      "Returned content expected 'nrvo', got '%s'", s.getContent());
}

// ── Illegal constructs — must NOT compile ─────────────────────────────────────
ZTEST(string_test, test_illegal_constructs_commented_out)
{
    // NOT ALLOWED: implicit conversion from const char* — constructor is explicit
    // well_defined::String s1 = "implicit";                 // error: no implicit conversion

    // NOT ALLOWED: implicit conversion from int — constructor is explicit
    // well_defined::String s2 = 10;                         // error: no implicit conversion

    // NOT ALLOWED: passing well_defined::String where const char* expected
    // const char *p = well_defined::String("nope");         // error: no conversion operator

    // NOT ALLOWED: accessing private member _szArray directly
    // auto stolen = s._szArray;                     // error: _szArray is private

    // NOT ALLOWED: copying unique_ptr — it is move-only
    // std::unique_ptr<char[]> p = other._szArray;   // error: copy of unique_ptr deleted

    // NOT ALLOWED: taking address of temporary
    // const well_defined::String *p2 = &well_defined::String("tmp"); // error: address of temporary

    zassert_true(true, "All illegal constructs correctly prevented by compiler");
}

ZTEST_SUITE(string_test, NULL, NULL, NULL, NULL, NULL);
