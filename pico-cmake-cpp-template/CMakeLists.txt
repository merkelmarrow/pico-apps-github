
cmake_minimum_required (VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# this must be done before project()
# must set PICO_SDK_PATH in your environment variables
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project ("pico-cmake-cpp-template")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (pico-cmake-cpp-template "pico-cmake-cpp-template.cpp" )

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add the standard library to the build
target_link_libraries(pico-cmake-cpp-template pico_stdlib)

# Replace "pico-cmake-cpp-template" with the appropriate target
pico_add_extra_outputs(pico-cmake-cpp-template)