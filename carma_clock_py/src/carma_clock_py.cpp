#include <pybind11/pybind11.h>
#include <string>
#include <carma_clock.h>
namespace py = pybind11;

PYBIND11_MODULE(libcarma_clock, m)
{
    m.doc() = R"(CARMA Time Module provides the CarmaClock object, which is a 
        wrapper for direct calls to system time. It is intended for use when it is 
        necessary to have control over each increment in time. An example of a use 
        case is in a simulation environment where time may progress non-linearly. 
        The CarmaClock constructor takes a boolean parameter to indicate whether 
        to directly make calls to system time or to use its internal store value
        for time. Calls to its update() method will update the internal stored value,
        while calls to its now* or sleep* will return or unblock threads depending 
        on the stored value respectively.)";

    py::class_<fwha_stol::lib::time::CarmaClock>(m, "CarmaClock")
        .def(py::init<bool>(), py::arg("simulation_mode") = false)
        .def("nowInSeconds", &fwha_stol::lib::time::CarmaClock::nowInSeconds ,
            R"(Get current time in seconds)")
        .def("nowInMilliseconds", &fwha_stol::lib::time::CarmaClock::nowInMilliseconds ,
            R"(Get current time in milliseconds)")
        .def("update", &fwha_stol::lib::time::CarmaClock::update, py::arg("current_time"),
            R"("Update current time to value (ms). Only valid for simulation mode True)")
        .def("is_simulation_mode", &fwha_stol::lib::time::CarmaClock::is_simulation_mode, 
            R"(Return True if simulation mode set to True and False if not.)")
        .def("wait_for_initialization", &fwha_stol::lib::time::CarmaClock::wait_for_initialization,
            R"(Method will block thread until update() is called on CarmaClock. 
            This method should be called once before any calls to sleep or now methods
            to ensure that time has a value.)")
        .def("sleep_until", &fwha_stol::lib::time::CarmaClock::sleep_until, py::arg("future_time"),
            R"(Method will block thread until given time (ms))")
        .def("sleep_for", &fwha_stol::lib::time::CarmaClock::sleep_for, py::arg("time_to_sleep"),
            R"(Method will block thread for given time (ms))");
}
