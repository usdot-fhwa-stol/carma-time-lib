cmake_minimum_required(VERSION 3.10.2)

# where to find our CMake modules
set(CMAKE_MODULE_PATH "$ENV{CARMA_OPT_DIR}/cmake")
# Required for std::lock and syntax
set(CMAKE_CXX_STANDARD 17)


project(carma_clock
    DESCRIPTION "CARMA time library"
    HOMEPAGE_URL https://github.com/usdot-fhwa-stol/carma-time-lib
	VERSION 0.0.1
    LANGUAGES CXX
)
include(cmake/dependencies.cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


add_library(${PROJECT_NAME} SHARED)
target_include_directories(${PROJECT_NAME} 
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
target_link_libraries( ${PROJECT_NAME} PUBLIC
    Boost::python
    Python3::Python
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION 1
)

include(CommonSource)
include(Testing)
# Test for Python inclusion
FILE(COPY python_wrapper_test.py DESTINATION .)
ADD_TEST(NAME python_wrapper_test COMMAND ${PYTHON_EXECUTABLE} python_wrapper_test.py)
include(CodeCoverage)
include(InstallingGeneral)
include(InstallingConfigs)
include(Packing)
