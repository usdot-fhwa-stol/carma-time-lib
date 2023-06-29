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

#include "carma_clock.h"

#include <thread>
#include <exception>

namespace fwha_stol::lib::time {

CarmaClock::CarmaClock(bool simulation_mode) : _is_simulation_mode(simulation_mode) {
    // ready to go if not sim mode
    _is_initialized = ! _is_simulation_mode;
}

timeStampMilliseconds CarmaClock::nowInMilliseconds() const {
    if (_is_simulation_mode) {
        if (!_is_initialized) {
            throw std::invalid_argument("Clock is not initialized!");
        }
        return _current_time;
    } else {
        using namespace std::chrono;
        return duration_cast< milliseconds>(system_clock::now().time_since_epoch()).count();
    }
}

timeStampSeconds CarmaClock::nowInSeconds() const {
    if (_is_simulation_mode) {
        if (!_is_initialized) {
            throw std::invalid_argument("Clock is not initialized!");
        }
        // purposefully round down as this is the same behavior as chrono::duration_cast
        return (_current_time / 1000);
    } else {
        using namespace std::chrono;
        return duration_cast< seconds>(system_clock::now().time_since_epoch()).count();
    }
}

void CarmaClock::update(timeStampMilliseconds current_time) {
    // check for sim time and throw exception if not in sim
    if (!_is_simulation_mode) {
        throw std::invalid_argument("Clock is not in simulation mode!");
    }
    _current_time = current_time;
    if (!_is_initialized) {
        std::unique_lock init_lk( _initialization_mutex );
        // std::cout << "Number of threads waiting on initialization : " << std::to_string(_initialization_cv_pairs.size()) << std::endl;
        for (auto iter = _initialization_cv_pairs.begin(); iter != _initialization_cv_pairs.end(); )
        {
            // Notify thread and remove pair
            std::unique_lock lock(*iter->second);
            iter->first->notify_one();
            iter = _initialization_cv_pairs.erase(iter);
            // std::cout << "Number of threads waiting on initialization : " << std::to_string(_initialization_cv_pairs.size()) << std::endl;

        }   
        // Once all waiting threads have been notified
        _is_initialized = true;
    }
    // check to see if any sleeping threads need to be woken
    std::unique_lock lk(_sleep_mutex);
    for (auto iter = _sleep_holder.begin(); iter != _sleep_holder.end(); )
    {
        if(_current_time >= iter->first) {
            // found one, notify and then remove the record
            std::unique_lock lock(*iter->second.second);
            iter->second.first->notify_one();
            iter = _sleep_holder.erase(iter);
        } else {
            iter++;
        }
    }    
}

void CarmaClock::wait_for_initialization() {
    if (!_is_initialized) {
        
        // create the CV and mutex to use
        sleepCVPair initializationCVPair = std::make_pair(
            std::make_shared<std::condition_variable>(),
            std::make_shared<std::mutex>()
        );
        {
            // Lock vector for modification and add pair
            std::unique_lock lk(_initialization_mutex);
            _initialization_cv_pairs.emplace_back(initializationCVPair);
        }
        // Set lock pair for modification and set thread to wait for initialization
        std::unique_lock lock(*initializationCVPair.second);
        initializationCVPair.first->wait(lock);
    }
}

void CarmaClock::sleep_for(timeStampMilliseconds time_to_sleep) {
    sleep_until( _current_time + time_to_sleep);
}

void CarmaClock::sleep_until(timeStampMilliseconds future_time) {
    if (!_is_initialized) {
        throw std::invalid_argument("Clock is not initialized!");
    }
    if (_is_simulation_mode) {
        // determine if we are at the time or not and skip sleep if past
        if (future_time > _current_time) {
            // create the CV and mutex to use
            sleepCVPair sleepCVPairValue = std::make_pair(
                std::make_shared<std::condition_variable>(),
                std::make_shared<std::mutex>()
            );
            {
                // add the time and the values to our list
                std::unique_lock lk(_sleep_mutex);
                _sleep_holder.emplace_back(future_time, sleepCVPairValue);
            }
            // wait for something to notify that this thread should proceed
            std::unique_lock lock(*sleepCVPairValue.second);
            sleepCVPairValue.first->wait(lock);
        }
    } else {
        // do system sleep
        using namespace std::chrono;
        std::chrono::system_clock::time_point futureTimePoint{std::chrono::milliseconds{future_time}};
        std::this_thread::sleep_until(futureTimePoint);
    }
}

}
