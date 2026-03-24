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
 * @file callback_lifetime.cpp
 * @author Serge Ayer <serge.ayer@hefr.ch>
 *
 * @brief Demonstrates lifetime and capture discipline.
 *
 *   - Dangling reference from capturing a local by reference
 *   - Capture by value as the safe alternative
 *   - shared_ptr capture for shared ownership across callbacks
 *   - Weak_ptr capture to avoid extending lifetime unintentionally
 *
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/

#include "callback_lifetime.hpp"

// zephyr
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// std
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

LOG_MODULE_DECLARE(programming, CONFIG_APP_LOG_LEVEL);

// ── Types ─────────────────────────────────────────────────────────────────────
enum class Event { Tick = 0, Shutdown = 1 };

using Callback = std::function<void(Event)>;

// ── Simple event dispatcher ──────────────────────────────────────────────────────────
class EventDispatcher {
 public:
  void add(Callback cb) { _callbacks.push_back(std::move(cb)); }

  void dispatch(Event e) {
    for (auto& cb : _callbacks) {
      cb(e);
    }
  }

 private:
  std::vector<Callback> _callbacks;
};

// ── Shared resource ───────────────────────────────────────────────────────────
struct SharedLog {
  std::vector<std::string> entries;
  std::string name;

  explicit SharedLog(std::string n) : name(std::move(n)) {}

  void append(const std::string& msg) {
    entries.push_back(msg);
    LOG_INF("[%s] logged: '%s'", name.c_str(), msg.c_str());
  }

  ~SharedLog() {
    LOG_INF("SharedLog [%s] destroyed - %zu entries", name.c_str(), entries.size());
  }
};

// ── 1. Dangling reference — what NOT to do ────────────────────────────────────
[[maybe_unused]] static Callback make_dangling_callback() {
  int local_counter = 100;

  // Captures local_counter by reference.
  // local_counter is destroyed when this function returns.
  // Calling the returned lambda is undefined behaviour.
  return [&local_counter](Event) {
    // local_counter is a dangling reference here
    LOG_INF("[value reference] counter = %d",
            local_counter);  // Undefined behavior — do not do this
  };
}

// ── 2. Capture by value — safe for locals ────────────────────────────────────
static Callback make_safe_callback_by_value() {
  int local_counter = 100;

  // Captures a copy of local_counter.
  // The lambda owns its copy — no dangling reference possible.
  return [local_counter](Event e) mutable {
    if (e == Event::Tick) {
      local_counter++;
      LOG_INF("[value capture] counter = %d", local_counter);
    }
  };
}

// ── 3. shared_ptr capture — shared ownership ─────────────────────────────────
static Callback make_shared_ownership_callback(std::shared_ptr<SharedLog> log) {
  // Lambda captures shared_ptr by value — extends lifetime of the log
  // for as long as the lambda exists.
  return [log](Event e) {
    if (e == Event::Tick) {
      log->append("tick received");
    } else if (e == Event::Shutdown) {
      log->append("shutdown received");
    }
  };
}

// ── 4. weak_ptr capture — observe without extending lifetime ──────────────────
static Callback make_weak_callback(std::weak_ptr<SharedLog> weak_log) {
  // Captures weak_ptr — does NOT extend the lifetime of SharedLog.
  // The callback checks whether the object still exists before using it.
  // This is the correct pattern when the callback should not prevent
  // the owner from destroying the resource.
  return [weak_log](Event e) {
    auto log = weak_log.lock();  // try to acquire strong reference
    if (!log) {
      LOG_INF("[weak capture] SharedLog already destroyed - skipping");
      return;
    }
    if (e == Event::Tick) {
      log->append("tick (weak)");
    }
  };
}

void callback_lifetime() {
  EventDispatcher dispatcher;

  // ── Dangling reference (commented out — undefined behavior if called)
  // ─────────────────────
  //
  LOG_INF("── Capture by reference ──");
  Callback dangling_cb = make_dangling_callback();
  dispatcher.add(dangling_cb);  // DO NOT do this — calling it is undefined behavior
  dispatcher.dispatch(Event::Tick);

  // ── Capture by value — safe ───────────────────────────────────────────────
  LOG_INF("── Capture by value ──");
  dispatcher.add(make_safe_callback_by_value());
  dispatcher.dispatch(Event::Tick);

  // ── shared_ptr capture ────────────────────────────────────────────────────
  LOG_INF("── shared_ptr capture ──");
  {
    auto shared_log = std::make_shared<SharedLog>("shared capture");

    // Two callbacks share ownership of the same log
    dispatcher.add(make_shared_ownership_callback(shared_log));
    dispatcher.add(make_shared_ownership_callback(shared_log));

    LOG_INF("shared_log use_count before publish: %ld",
            shared_log.use_count());  // local + 2 lambdas

    dispatcher.dispatch(Event::Tick);

    LOG_INF("shared_log use_count after publish: %ld",
            shared_log.use_count());  // identical: local + 2 lambdas

    // shared_log goes out of scope here but the lambdas in dispatcher still
    // hold shared_ptr copies — SharedLog is NOT destroyed yet
  }
  LOG_INF("shared_log local reference gone - lambdas still hold it alive");

  // ── weak_ptr capture ──────────────────────────────────────────────────────
  LOG_INF("── weak_ptr capture ──");
  {
    auto observable_log = std::make_shared<SharedLog>("observable");
    dispatcher.add(make_weak_callback(observable_log));

    LOG_INF("observable_log alive - dispatching tick:");
    dispatcher.dispatch(Event::Tick);

    // Destroy observable_log — weak_ptr callback should detect this
    LOG_INF("Destroying observable_log...");
    observable_log.reset();  // reference count → 0, object destroyed

    LOG_INF("observable_log destroyed - dispatching tick:");
    dispatcher.dispatch(Event::Tick);  // weak_ptr callback gracefully skips
  }

  // ── Shutdown event ────────────────────────────────────────────────────────
  LOG_INF("── Dispatching shutdown ──");
  dispatcher.dispatch(Event::Shutdown);
}
