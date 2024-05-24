#include <pybind11/pybind11.h>

#include <carma_clock.h>
namespace py = pybind11;

PYBIND11_MODULE(carma_clock_py, m) {
    py::class_<fwha_stol::lib::time::CarmaClock>(m, "CarmaClock")
        .def(py::init<bool>(), py::arg("simulation_mode")=false)
        .def("nowInSeconds", &fwha_stol::lib::time::CarmaClock::nowInSeconds)
        .def("nowInMilliseconds", &fwha_stol::lib::time::CarmaClock::nowInMilliseconds)
        .def("update", &fwha_stol::lib::time::CarmaClock::update, py::arg("current_time"))
        .def("is_simulation_mode", &fwha_stol::lib::time::CarmaClock::is_simulation_mode)
        .def("wait_for_initialization",&fwha_stol::lib::time::CarmaClock::wait_for_initialization)
        .def("sleep_until",&fwha_stol::lib::time::CarmaClock::sleep_until, py::arg("future_time"))
        .def("sleep_for",&fwha_stol::lib::time::CarmaClock::sleep_for, py::arg("time_to_sleep"));

}
