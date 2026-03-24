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
 * @file callback_lifetime.hpp
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
 *
 * @date 2025-07-01
 * @version 1.0.0
 ***************************************************************************/

#pragma once

void callback_lifetime();
