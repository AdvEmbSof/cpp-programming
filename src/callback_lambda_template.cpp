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
 * @file callback_lambda_template.cpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief callback with capturing lambda function implementation
 *        As compared to callback_lambda, the capturing lambda
 *        is passed to a template parameter. No std::function used anywhere.
 *        The lambda type is fully known at compile time and the compiler
 *        can inline the call. This is a zero-cost alternative.
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/

#include "callback_lambda_template.hpp"

// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// std
#include <cstdint>
#include <cstdio>
#include <utility>

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

// ── Types ─────────────────────────────────────────────────────────────────────

enum class Event {
  None     = 0,
  Pressed  = 1,
  Released = 2,
};

struct ButtonState {
  int id                 = 0;
  unsigned press_count   = 0;
  unsigned release_count = 0;
};

// ── Hardware abstraction — template callback, no std::function ────────────────

/**
 * ButtonDriver stores the callback as a template parameter.
 * The lambda type is fully resolved at compile time.
 * No type erasure. No heap allocation. No virtual dispatch.
 *
 * In C this would be:
 *   void register_callback(callback_t cb, void *ctx);
 *   register_callback(my_handler, &my_state);
 *
 * Here the "context" is the lambda capture list — type safe, no cast needed.
 */
template <typename OnEvent>
class ButtonDriver {
 public:
  explicit ButtonDriver(int hw_id, OnEvent on_event)
      : _hw_id(hw_id), _on_event(std::move(on_event)) {}

  // Simulate hardware interrupt — calls callback directly, compiler inlines it
  void simulate_press() { _on_event(Event::Pressed); }
  void simulate_release() { _on_event(Event::Released); }

  int id() const { return _hw_id; }

 private:
  int _hw_id;
  OnEvent _on_event;  // concrete lambda type — NOT std::function
};

void callback_lambda_template() {
  ButtonState btn_a{.id = 0};
  ButtonState btn_b{.id = 1};

  // ── C style (before) ──────────────────────────────────────────────────────
  // void *ctx = &btn_a;
  // register_callback(button_handler, ctx);
  // Inside button_handler: ButtonState *s = (ButtonState *)ctx;  ← unsafe cast
  //
  // ── C++ style (after) — lambda captures state, no void*, no cast ─────────

  ButtonDriver driver_a(0, [&btn_a](Event e) {
    // btn_a is captured by reference — type safe, no cast
    switch (e) {
      case Event::Pressed:
        btn_a.press_count++;
        LOG_INF("[button %d] pressed  (total: %u)", btn_a.id, btn_a.press_count);
        break;
      case Event::Released:
        btn_a.release_count++;
        LOG_INF("[button %d] released (total: %u)", btn_a.id, btn_a.release_count);
        break;
      default:
        break;
    }
  });

  ButtonDriver driver_b(1, [&btn_b](Event e) {
    switch (e) {
      case Event::Pressed:
        btn_b.press_count++;
        LOG_INF("[button %d] pressed  (total: %u)", btn_b.id, btn_b.press_count);
        break;
      case Event::Released:
        btn_b.release_count++;
        LOG_INF("[button %d] released (total: %u)", btn_b.id, btn_b.release_count);
        break;
      default:
        break;
    }
  });

  // Simulate events
  driver_a.simulate_press();
  driver_b.simulate_press();
  driver_a.simulate_press();
  driver_a.simulate_release();

  LOG_INF("callback_lambda_template: Final counts: btn_a pressed=%u  btn_b pressed=%u",
          btn_a.press_count,
          btn_b.press_count);

  // In this example, ButtonDriver<lambda_type_A> and ButtonDriver<lambda_type_B>
  // have different types.
  // This approach is therefore not adequate when callbacks need to be stored
  // in the same array or vector (heterogeneous storage). In this
  // case std::function is needed, as in the callback_lambda implementation.
}
