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

include(CommonSource)
if (DEFINED ENV{BUILD_ARCHITECTURE})
    if ($ENV{BUILD_ARCHITECTURE} STREQUAL "x64") 
        message(STATUS "Building Python Module binding only supported for x64 builds...")
        add_subdirectory(carma_clock_py)
    else() 
        message(STATUS "Building Python Module binding NOT supported for " ${BUILD_ARCHITECTURE} " builds...")
    endif()
else()
    message(STATUS "Enviroment variable for BUILD_ARCHITECTURE is not set. Skipping Python Module binding ")
endif()

# message(STATUS "Building Python Module binding only supported for x64 builds...")
include(Testing)
include(CodeCoverage)
include(InstallingGeneral)
include(InstallingConfigs)
include(Packing)


