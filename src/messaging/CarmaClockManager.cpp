#include "messaging/CarmaClockManager.h"

CarmaClockManager::CarmaClockManager(std::shared_ptr<CarmaClock>& clock) : _clock(clock) {}

void CarmaClockManager::handle_time_sync_message(timestamp) {
    _clock->update_simtime(timestamp timestamp);
}
