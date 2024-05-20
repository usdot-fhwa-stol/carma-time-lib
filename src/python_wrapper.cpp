#include <boost/python.hpp>
#include <boost/python/def.hpp>
#include "carma_clock.h"

BOOST_PYTHON_MODULE(libcarma_clockd) {
    using namespace boost::python;
    class_<fwha_stol::lib::time::CarmaClock, boost::noncopyable>("CarmaClock", init<bool>())
        .def("nowInSeconds", &fwha_stol::lib::time::CarmaClock::nowInSeconds)
        .def("nowInMilliseconds", &fwha_stol::lib::time::CarmaClock::nowInMilliseconds)
        .def("update", &fwha_stol::lib::time::CarmaClock::update,args("current_time"))
        .def("is_simulation_mode", &fwha_stol::lib::time::CarmaClock::is_simulation_mode)
        .def("wait_for_initialization",&fwha_stol::lib::time::CarmaClock::wait_for_initialization)
        .def("sleep_until",&fwha_stol::lib::time::CarmaClock::sleep_until,args("future_time"))
        .def("sleep_for",&fwha_stol::lib::time::CarmaClock::sleep_for,args("time_to_sleep"));
        init_module_libcarma_clockd();
}