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

#ifndef CARMA_TIME_LIB_CARMACLOCKMANAGER_H
#define CARMA_TIME_LIB_CARMACLOCKMANAGER_H

namespace fwha_stol::lib::time::messaging {

    class CarmaClockManager {
    protected:
        // Ensure this function is called whenever a simulation time sync message is received
        void handle_time_sync_message(timestamp timestamp);

    protected:
        std::shared_ptr <CarmaClock> _clock;
    };

}

#endif //CARMA_TIME_LIB_CARMACLOCKMANAGER_H
