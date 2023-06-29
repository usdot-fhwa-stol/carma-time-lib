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

#include <memory>
#include <condition_variable>
#include <mutex>
#include <vector>
// Used for cout statemtents for debugging
// #include <iostream>

namespace fwha_stol::lib::time {

/** Alias for a timestamp in seconds. */
using timeStampSeconds = uint32_t;
/** Alias for a timestamp in milliseconds. */
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
    /** Create a clock instance using read time mode (default) or simulation mode */
    explicit CarmaClock(bool simulation_mode = false);

    ~CarmaClock() = default;

public:
    /** Get the current epoch time in seconds, floored */
    timeStampSeconds nowInSeconds() const;
    /** Get the current epoch time in milliseconds */
    timeStampMilliseconds nowInMilliseconds() const;

public:
    /** Update the clock time, only valid when in simulation mode */
    void update(timeStampMilliseconds current_time);

public:
    /** Return whether this instance is in simulation mode */
    inline bool is_simulation_mode() const {return _is_simulation_mode; };

    /** Block until the clock has been initialized.  For simulations nothing should
        happen until the clock starts. */
    void wait_for_initialization();

    /** Sleep until a future point in time. */
    void sleep_until(timeStampMilliseconds future_time);
    /**
     * @brief Sleep for time_to_sleep in milliseconds.
     * @param time_to_sleep milliseconds from current time to sleep for.
    */
    void sleep_for(timeStampMilliseconds time_to_sleep);

private:
    // Current simulation time
    timeStampMilliseconds _current_time = 0;

    // Simulation mode
    bool _is_simulation_mode;

    // Initialization variables
    bool _is_initialized;
    // Vector of conditional_variable mutex pairs to handle multiple threads waiting on initialization
    std::vector< sleepCVPair> _initialization_cv_pairs;
    // Mutex for vector of conditional variable mutex pairs
    std::mutex _initialization_mutex;

    // sleep related items
    std::mutex _sleep_mutex;
    std::vector <sleepValuePair> _sleep_holder;
};

}
