cmake_minimum_required(VERSION 3.10.2)

# where to find our CMake modules
set(CMAKE_MODULE_PATH "$ENV{CARMA_OPT_DIR}/cmake")
# Required for std::lock and syntax
set(CMAKE_CXX_STANDARD 17)


project(carma-clock
    DESCRIPTION "CARMA time library"
    HOMEPAGE_URL https://github.com/usdot-fhwa-stol/carma-time-lib
	VERSION 0.0.1
    LANGUAGES CXX
)

option(BUILD_PYTHON_BINDINGS 
    "BUILD PYTHON BINDINGS is used to configure whether or not to including building python module bindings into the cmake build process.
     **NOTE** Build python bindings is currently only support for native builds (not supported for cross-compile builds) " OFF)

include(cmake/dependencies.cmake)
add_library(${PROJECT_NAME} SHARED )

target_include_directories(${PROJECT_NAME} 
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION 1
)

include(CommonSource)
include(Testing)
include(CodeCoverage)
include(InstallingGeneral)
include(InstallingConfigs)

# Build customization for BUILD_PYTHON_BINDINGS option
if (${BUILD_PYTHON_BINDINGS})
    # BUILD_ARCHIECTURE is an environment variable set in carma-builds base image for cross-compile targets. It is unset
    # for native builds. Since python module binding does not work for cross-compiled targets, if build architecture is
    # set, we must turn off build python bindings. (carma-builds repo https://github.com/usdot-fhwa-stol/carma-builds)
    #TODO Move this check from CMAKE file to build.sh file to remove coupling of CMake file to carma-builds base image  
    if (DEFINED ENV{BUILD_ARCHITECTURE})
        set(BUILD_PYTHON_BINDINGS OFF)
        message(WARNING "BUILD_PYTHON_BINDINGS option NOT supported for cross compiled targets (" $ENV{BUILD_ARCHITECTURE} ") ...")
    endif()
endif()
if (${BUILD_PYTHON_BINDINGS})
    message(VERBOSE "Adding dependencies and tests for building python module bindings (set BUILD_PYTHON_BINDINGS=OFF to prevent python module binding build) ...")
    include(carma_clock_py/cmake/dependencies.cmake)
    target_link_libraries( ${PROJECT_NAME} PUBLIC
        Python3::Python
        pybind11::module
    )
    # set up sources based on target name
    target_sources (${PROJECT_NAME}
        PRIVATE
            carma_clock_py/src/carma_clock_py.cpp
    )
    # set up test for python module binding 
    FILE(COPY carma_clock_py/python_wrapper_test.py DESTINATION .)
    add_test(NAME test_carma_clock_python_module_binding COMMAND ${PYTHON_EXECUTABLE} python_wrapper_test.py )
    # set cpack depedencies
    set(CPACK_DEBIAN_PACKAGE_DEPENDS python3-dev)
    # remove debug post fix for library. Required due to import naming for python module import
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "")
endif()

include(Packing)



