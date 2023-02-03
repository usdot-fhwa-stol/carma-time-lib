#include "CarmaClock.h"

using fwha_stol::lib::time;

static final duration CarmaClock::DEFAULT_LOOP_DELAY = 10;

CarmaClock::CarmaClock(bool enable_simulation_mode) :
        _reference_clock(std::make_shared<reference_clock_type>()),
        _current_sim_time(_reference_clock.now()),
        _is_simulation_mode(enable_simulation_mode),
        _periodic_callback(null_ptr),
        _periodic_delay(0),
        _periodic_num_repetitions(-1),
        _is_initialized(false)
{
    if (enable_simulation_mode) {
        initialize_with_simulation();
    } else {
        initialize_without_simulation();
    }
}

CarmaClock::CarmaClock(bool enable_simulation_mode, timestamp current_time) :
        _reference_clock(std::make_shared<reference_clock_type>()),
        _current_sim_time(current_time),
        _is_simulation_mode(enable_simulation_mode),
        _periodic_callback(null_ptr),
        _periodic_delay(0),
        _periodic_num_repetitions(-1),
        _is_initialized(false)
{}

~CarmaClock::CarmaClock() {}

timestamp CarmaClock::now() {
    if (_is_simulation_mode) {
        return _current_sim_time;
    } else {
        return _reference_clock.get
    }
}

bool CarmaClock::is_simulation_mode() {
    return _is_simulation_mode;
}

void CarmaClock::update_sim_time(timestamp current_sim_time) {
    _current_sim_time = current_sim_time;
}

void CarmaClock::wait_for_initialization() {
    // Block calling thread until some other thread mutates the clock's _is_initialized parameter by calling
    // an initialize_*() method. TODO Will this work or have side effects?
    while (!_is_initialized) {
        usleep(DEFAULT_LOOP_DELAY);
    }
}

void CarmaClock::sleep(duration) {
    duration future_time = _current_sim_time + duration;
    while (_current_sim_time < future_time) {
        usleep(DEFAULT_LOOP_DELAY);  // TODO Is there a better way to do this?
    }
}

void CarmaClock::register_periodic_callback(duration periodic_delay, void_callback fcn, int num_repititions) {
    _periodic_callback = fcn;
    _periodic_delay = periodic_delay;
    _periodic_num_repetitions = num_repititions;
}

void CarmaClock::initialize_with_simulation(timestamp current_sim_time) {
    _current_sim_time = current_sim_time
    _is_simulation_mode = true;
    _is_initialized = true;
}

void CarmaClock::initialize_without_simulation() {
    _current_sim_time = _reference_clock.now();
    _is_simulation_mode = false;
    _is_initialized = true;
}
