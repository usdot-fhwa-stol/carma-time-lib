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
add_library(${PROJECT_NAME} SHARED )

target_include_directories(${PROJECT_NAME} 
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION 1
)

if (DEFINED ENV{BUILD_ARCHITECTURE})
    message(STATUS "Building Python Module binding NOT supported for crosss compiling " $ENV{BUILD_ARCHITECTURE} " builds...")
else()
    include(carma_clock_py/cmake/dependencies.cmake)
    target_link_libraries( ${PROJECT_NAME} PUBLIC
        Python3::Python
        pybind11::module
    )
    message(STATUS "Building Python Module binding only supported for native x64 builds...")
    # set up sources based on target name
    target_sources (${PROJECT_NAME}
        PRIVATE
            carma_clock_py/src/carma_clock_py.cpp
    )
    FILE(COPY carma_clock_py/python_wrapper_test.py DESTINATION .)
    add_test(NAME test_carma_clock_python_module_binding COMMAND ${PYTHON_EXECUTABLE} python_wrapper_test.py )
endif()

include(CommonSource)
include(Testing)
include(CodeCoverage)
include(InstallingGeneral)
include(InstallingConfigs)
set(CPACK_DEBIAN_PACKAGE_DEPENDS carma-clock-1 python3-dev)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "")

include(Packing)


