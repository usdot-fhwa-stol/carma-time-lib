cmake_minimum_required(VERSION 3.10.2)

# where to find our CMake modules
set(CMAKE_MODULE_PATH "$ENV{CARMA_OPT_DIR}/cmake")

project(carma-clock
    DESCRIPTION "CARMA time library"
	VERSION 0.0.1
)

add_library(${PROJECT_NAME} SHARED)

include(CommonSource)
include(Testing)
include(InstallingGeneral)
include(InstallingConfigs)
include(Packing)
