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

void CarmaClock::update(timeStampMilliseconds current_time) {
    _current_time = current_time;
    if (!_is_initialized) {
        // if not initialized then do it and let anyone waiting know
        _is_initialized = true;
        std::unique_lock lock(_initialization_mutex);
        _initialization_cv.notify_one();
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
        std::unique_lock lock(_initialization_mutex);
        _initialization_cv.wait(lock, [this] { return _is_initialized; });
    }
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
