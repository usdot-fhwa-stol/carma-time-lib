# CARMA Clock Python Module
This directory includes the source files for building the CARMA Clock Python Module bindings with [pybind11](https://pybind11.readthedocs.io/en/stable/). It includes a `src/carma_clock-py.cpp` file which calls the pybind11 library functions/macros to build python module bindings and a `/cmake/dependencies.cmake` file which pulls in pybind11 and finds python3 for CMake. It also includes a CMake test for which runs the `python_wrapper.test.py` to ensure the resulting library built can be imported as a python module.
## Dependencies
The CMake dependencies script relies on [CPM](https://github.com/cpm-cmake/CPM.cmake) and Python3 to be installed and discoverable to CMake.

