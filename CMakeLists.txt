
# To use with the CMakePresets.json file, the following have to be in your systems PATH:
# arm-none-eabi-gcc
# arm-none-eabi-g++
# mingw32-make

# PICO_SDK_PATH must be set in your environment variables

# CMake minimum version must be 3.13 or higher in order to use the Pico SDK
cmake_minimum_required (VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# this must be done before project()
# must set PICO_SDK_PATH in your environment variables
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project ("pico-cmake-cpp-template")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# used by pico_set_program_url
set(PICO_APPS_PATH ${PROJECT_SOURCE_DIR})

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include(apps_auto_set_url.cmake)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_subdirectory(labs)