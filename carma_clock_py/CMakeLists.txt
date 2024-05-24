cmake_minimum_required(VERSION 3.10.2)

# where to find our CMake modules
set(CMAKE_MODULE_PATH "$ENV{CARMA_OPT_DIR}/cmake")


project(carma_clock_py
    DESCRIPTION "CARMA time library Python Module"
    HOMEPAGE_URL https://github.com/usdot-fhwa-stol/carma-time-lib
	VERSION 0.0.1
    LANGUAGES CXX
)

include(cmake/dependencies.cmake)
pybind11_add_module(${PROJECT_NAME} SHARED src/carma_clock_py )

target_include_directories(${PROJECT_NAME} 
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_link_libraries( ${PROJECT_NAME} PUBLIC
    Python3::Python
    pybind11::module
    carma_clock

)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION 1
)

#Test for Python inclusion
FILE(COPY python_wrapper_test.py DESTINATION .)
add_custom_target(test_carma_clock_python_module_binding ALL COMMAND ${PYTHON_EXECUTABLE} python_wrapper_test.py DEPENDS ${PROJECT_NAME})



