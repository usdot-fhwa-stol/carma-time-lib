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
// TODO Ensure license at the top is correct.

#pragma once

#include <cstdint>
#include <chrono>
#include <memory>
#include <condition_variable>
#include <mutex>
#include <vector>

namespace fwha_stol::lib::time {

//typedef std::chrono::system_clock reference_clock_type;
//typedef std::chrono::time_point <reference_clock_type> timestamp;  //TODO Should we bother to use system-agnostic representation?
//typedef std::chrono::duration <reference_clock_type> duration;

using timeStampSeconds = uint32_t;
using timeStampMilliseconds = uint64_t;
using sleepCVPair = std::pair<std::shared_ptr<std::condition_variable>, std::shared_ptr<std::mutex>>;
using sleepValuePair = std::pair<timeStampMilliseconds, sleepCVPair>;

/**
 * Provides access to current time and various utilities. If a simulation is enabled, all time-related
 * functions reflect simulation time. Otherwise system time (via std::chrono::system_clock) is utilized.
 *
 * No interpolation is performed; the most recently received simulation time point is considered to
 * be current.
 */
class CarmaClock {
public:
    CarmaClock(bool simulation_mode = false);

    ~CarmaClock() = default;

public:
    // Time interrogation
    timeStampSeconds nowInSeconds();
    timeStampMilliseconds nowInMilliseconds() const;

public:
    // Time management updates
    void update(timeStampMilliseconds current_time);

public:
    // Toggle simulation
    void enable_simulation_mode();

    void disable_simulation_mode();

    inline bool is_simulation_mode() const { return _is_simulation_mode; };

    void wait_for_initialization();

    void sleep_until(timeStampMilliseconds future_time);

    // TODO Do we want register_periodic_callback?

private:
    // Current simulation time
    timeStampMilliseconds _current_time = 0;

    // Simulation mode
    bool _is_simulation_mode;

    // Initialization variables
    bool _is_initialized;
    std::mutex _initialization_mutex;
    std::condition_variable _initialization_cv;

    // sleep related items
    std::mutex _sleep_mutex;
    std::vector <sleepValuePair> _sleep_holder;

//    // System clock reference
//    std::shared_ptr<reference_clock_type> _reference_clock;
};

}
