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
 * @file callback_lambda.cpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief callback with lambda function implementation
 *        As compared to callback_fp, replace void* context + function pointer with a
 *        capturing lambda. The capture list carries state — no cast needed.
 * 
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/
// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// std
#include <cstdio>
#include <functional>

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

// ── Types ─────────────────────────────────────────────────────────────────────
enum class Event {
    None     = 0,
    Pressed  = 1,
    Released = 2,
};

struct ButtonState {
    int      id            = 0;
    unsigned press_count   = 0;
    unsigned release_count = 0;
};

// ── Callback registry ─────────────────────────────────────────────────────────
// Uses std::function here only to hold heterogeneous lambdas in the registry.
static constexpr int MAX_CALLBACKS = 8;

using Callback = std::function<void(Event)>;

static Callback registry[MAX_CALLBACKS];
static int      registry_count = 0;

static void register_callback(Callback cb)
{
    if (registry_count >= MAX_CALLBACKS) {
        LOG_ERR("ERROR: callback registry full");
        return;
    }
    registry[registry_count++] = std::move(cb);
}

static void dispatch_event(Event event)
{
    for (int i = 0; i < registry_count; i++) {
        if (registry[i]) {
            registry[i](event);
        }
    }
}

int callback_lambda()
{
    ButtonState btn_a{ .id = 0 };
    ButtonState btn_b{ .id = 1 };

    // ── Before: C style ───────────────────────────────────────────────────────
    // register_callback(button_handler, &btn_a);   // void* — unsafe cast inside
    //
    // ── After: capturing lambda ───────────────────────────────────────────────
    // The lambda captures btn_a by reference.
    // No void* parameter, no cast, full type safety.

    register_callback([&btn_a](Event e) {
        switch (e) {
            case Event::Pressed:
                btn_a.press_count++;
                LOG_INF("[button %d] pressed  (total: %u)",
                        btn_a.id, btn_a.press_count);
                break;
            case Event::Released:
                btn_a.release_count++;
                LOG_INF("[button %d] released (total: %u)",
                        btn_a.id, btn_a.release_count);
                break;
            default:
                break;
        }
    });

    register_callback([&btn_b](Event e) {
        switch (e) {
            case Event::Pressed:
                btn_b.press_count++;
                LOG_INF("[button %d] pressed  (total: %u)",
                        btn_b.id, btn_b.press_count);
                break;
            case Event::Released:
                btn_b.release_count++;
                LOG_INF("[button %d] released (total: %u)",
                        btn_b.id, btn_b.release_count);
                break;
            default:
                break;
        }
    });

    // A stateless lambda — equivalent to a plain function pointer, zero overhead
    register_callback([](Event e) {
        LOG_INF("[logger] event=%d", static_cast<int>(e));
    });

    // Simulate events
    dispatch_event(Event::Pressed);
    dispatch_event(Event::Pressed);
    dispatch_event(Event::Released);

    LOG_INF("Final counts: btn_a pressed=%u  btn_b pressed=%u",
            btn_a.press_count, btn_b.press_count);

    // The callback must be of type Callback (std::function<void(Event)>)
    // It is not possible to pass a callback of another type like
    // register_callback([](int e) {
    //
    //});

    return 0;
}