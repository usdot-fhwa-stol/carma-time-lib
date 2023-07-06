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

namespace fwha_stol::lib::time {

/** Alias for a timestamp in seconds. */
using timeStampSeconds = uint32_t;
/** Alias for a timestamp in milliseconds. */
using timeStampMilliseconds = uint64_t;
using sleepCVPair = std::pair<std::shared_ptr<std::condition_variable>, std::shared_ptr<std::mutex>>;
using sleepValuePair = std::pair<timeStampMilliseconds, sleepCVPair>;

/**
 * @brief Provides access to current time and various utilities. If a simulation is enabled, all time-related
 * functions reflect simulation time. Otherwise system time (via std::chrono::system_clock) is utilized.
 *
 * No interpolation is performed; the most recently received simulation time point is considered to
 * be current.
 */
class CarmaClock {
public:
    /**
     * @brief Create a clock instance using read time mode (default) or simulation mode.
     * @param simulation_mode bool flag to indicate whether clock is in simulation mode.
     */
    explicit CarmaClock(bool simulation_mode = false);
    /**
     * @brief Destructor
    */
    ~CarmaClock() = default;

public:
    /**
     * @brief Get the current time in seconds.
     * @return epoch time in seconds as a uint32_t.
    */
    timeStampSeconds nowInSeconds() const;
    /**
     * @brief Get the current time in milliseconds.
     * @return epoch time in milliseconds as a uint64_t.
    */
    timeStampMilliseconds nowInMilliseconds() const;

public:
    /**
     * @brief Update the clock time. First call to update will initialize the carma-clock
     * @attention only valid when in simulation mode.
     * @param current_time in milliseconds to update the clock to.
     * @throws std::invalid_argument exception if called while carma-clock is not in simulation
     * mode.
    */
    void update(timeStampMilliseconds current_time);

public:
    
    /**
     * @brief Return whether this instance is in simulation mode.
    */
    inline bool is_simulation_mode() const {return _is_simulation_mode; };

    /**
     * @brief Will block calling thread until carma-clock has been initiliazed 
     * with the first call to update. 
     * @attention Only relavent when clock is in simulation mode. If not, will
     * immediately unblock.
    */
    void wait_for_initialization();
    /**
     * @brief Sleep until future time.
     * @param future_time epoch timestamp in milliseconds to sleep until.
     * @throw std::invalid_argument if carma-clock is not initialized.
     */
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
    // Mutex for conditional variable
    std::mutex _initialization_mutex;
    // Conditional_variable used to block one or multiple threads until 
    // carma-clock object is initialized. On initialization all threads are 
    // notified.
    std::condition_variable _initialization_cv;

    // sleep related items
    // Mutex for conditional variable
    std::mutex _sleep_mutex;
    // A vector of time, mutex and conditional variables used to
    // block calling threads until a certain time update. Each call 
    // to sleep_until, creates a new entry that includes a time, a conditional 
    // variable and a mutex for the conditional variable. On each incoming time update
    // , all vector entries with times smaller or equal to the incoming update are notified. 
    // NOTE: Each entry is only expected to have a single thread waiting on it so the relevant
    // time update will only notify a single thread for each entry. Multiple threads waiting on 
    // the same time should each have their one entry in this vector.
    std::vector <sleepValuePair> _sleep_holder;
};

}
