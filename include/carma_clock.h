/*
 * Copyright (C) 2023 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#pragma once

#include <cstdint>
#include <chrono>

namespace fwha_stol {
namespace lib {
namespace time {

typedef std::chrono::system_clock reference_clock_type;
typedef std::chrono::time_point <reference_clock_type> timestamp;
typedef std::chrono::duration <reference_clock_type> duration;

/**
 * Provides access to current time and various utilities. If a simulation is enabled, all time-related
 * functions reflect simulation time. Otherwise system time (via std::chrono::system_clock) is utilized.
 *
 * No interpolation is performed; the most recently received simulation time point is considered to
 * be current.
 */
class CarmaClock {
public:
    CarmaClock(bool enable_simulation_mode);

    CarmaClock(bool enable_simulation_mode, timestamp current_time);

    ~CarmaClock();

public:
    // Time interrogation
    timestamp now();

public:
    // Time management updates
    void update(timestamp current_time);

public:
    // Toggle simulation
    void enable_simulation_mode();

    void disable_simulation_mode();

    bool is_simulation_mode();

private:
    initialize();

private:
    // System clock reference
    std::shared_ptr <reference_clock_type> _clock;

    // Current simulation time
    timestamp current_time;

    // Simulation mode
    bool _is_simulation_mode;
};

}
}
}
