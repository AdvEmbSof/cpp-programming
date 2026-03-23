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
 * @file callback_fp.hpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief callback with function pointer implementation
 *        Classic C callback pattern: function pointer + void* context.
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/

#include "callback_fp.h"

// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// std
#include <stdio.h>
#include <stdlib.h>

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

// ── Types ─────────────────────────────────────────────────────────────────────
typedef enum {
  EVENT_NONE     = 0,
  EVENT_PRESSED  = 1,
  EVENT_RELEASED = 2,
} event_t;

typedef struct {
  int id;
  unsigned press_count;
  unsigned release_count;
} button_state_t;

// Callback signature: takes opaque context pointer and event code
typedef void (*callback_t)(void* ctx, int event);

// ── Callback registry ─────────────────────────────────────────────────────────
typedef struct {
  callback_t cb;
  void* ctx;
} registry_entry_t;

#define MAX_CALLBACKS 8

static registry_entry_t registry[MAX_CALLBACKS];
static int registry_count = 0;

static void register_callback(callback_t cb, void* ctx) {
  if (registry_count >= MAX_CALLBACKS) {
    LOG_ERR("ERROR: callback registry full");
    return;
  }
  registry[registry_count].cb  = cb;
  registry[registry_count].ctx = ctx;
  registry_count++;
}

static void dispatch_event(int event) {
  for (int i = 0; i < registry_count; i++) {
    if (registry[i].cb) {
      registry[i].cb(registry[i].ctx, event);
    }
  }
}

// ── Callbacks ─────────────────────────────────────────────────────────────────

// Handler for button A
static void button_handler(void* ctx, int event) {
  // ctx must be cast back to the concrete type — not type safe
  button_state_t* state = (button_state_t*)ctx;

  switch ((event_t)event) {
    case EVENT_PRESSED:
      state->press_count++;
      LOG_INF("[button %d] pressed  (total: %u)", state->id, state->press_count);
      break;
    case EVENT_RELEASED:
      state->release_count++;
      LOG_INF("[button %d] released (total: %u)", state->id, state->release_count);
      break;
    default:
      break;
  }
}

// A second handler registered for the same events
static void logger_handler(void* ctx, int event) {
  const char* label = (const char*)ctx;
  LOG_INF("[logger '%s'] event=%d", label, event);
}

// Function called from main()
void callback_fp(void) {
  button_state_t btn_a = {.id = 0, .press_count = 0, .release_count = 0};
  button_state_t btn_b = {.id = 1, .press_count = 0, .release_count = 0};

  // Register handlers — each carries its own context via void*
  register_callback(button_handler, &btn_a);
  register_callback(button_handler, &btn_b);
  register_callback(logger_handler, (void*)"audit");

  // Simulate events
  dispatch_event(EVENT_PRESSED);
  dispatch_event(EVENT_PRESSED);
  dispatch_event(EVENT_RELEASED);

  LOG_INF("Final counts: btn_a pressed=%u btn_b pressed=%u\n",
          btn_a.press_count,
          btn_b.press_count);

  // Registering the following callbacks is valid but may cause a crash
  // This shows that type-unsafe cast are possible
  // int* a = NULL;
  // register_callback(button_handler, a);
  // register_callback(logger_handler, &a);
  // dispatch_event(EVENT_RELEASED);
}
