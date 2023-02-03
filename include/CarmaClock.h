/**
 * Copyright (C) 2023 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this plogFile except in compliance with the License. You may obtain a copy of
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

#ifndef CARMA_TIME_LIB_CARMACLOCK_H
#define CARMA_TIME_LIB_CARMACLOCK_H

#include <cstdint>
#include <chrono>

namespace fwha_stol::lib::time {

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
    class CarmaClock : public CarmaScheduler {
    public:
        CarmaClock(bool enable_simulation_mode);

        CarmaClock(bool enable_simulation_mode, timestamp current_time);

        ~CarmaClock();

    public:
        // Time interrogation
        timestamp now();

        bool is_simulation_mode();

        // Time management updates
        void update_sim_time(timestamp current_sim_time);

        // CarmaScheduler interface
        void wait_for_initialization();

        void sleep(duration);

        void register_periodic_callback(duration periodic_delay, void_callback fcn, int num_repititions);

    private:
        // Initialization
        void initialize_with_simulation(timestamp current_sim_time);

        void initialize_without_simulation();

    private:
        // System clock reference
        std::shared_ptr<reference_clock_type> _reference_clock;

        // Current simulation time
        timestamp _current_sim_time;

        // Simulation mode
        bool _is_simulation_mode;

        // Scheduling members
        void_callback _periodic_callback;
        duration _periodic_delay;
        int _periodic_num_repetitions;
        bool _is_initialized;

        // Constants
        static final duration DEFAULT_LOOP_DELAY;
    };

}

#endif //CARMA_TIME_LIB_CARMACLOCK_H
