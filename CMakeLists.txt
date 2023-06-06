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
find_package(Threads)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


add_library(${PROJECT_NAME} SHARED)
set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION 1
)

include(CommonSource)
include(Testing)
include(CodeCoverage)
include(InstallingGeneral)
include(InstallingConfigs)
include(Packing)
