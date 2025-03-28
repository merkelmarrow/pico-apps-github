# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# CMake minimum version must be 3.13 or higher in order to use the Pico SDK
cmake_minimum_required (VERSION 3.13)

if(NOT DEFINED CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER "arm-none-eabi-gcc" CACHE STRING "C Compiler for Pico" FORCE)
endif()
if(NOT DEFINED CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER "arm-none-eabi-g++" CACHE STRING "C++ Compiler for Pico" FORCE)
endif()

# Define the Pico SDK path
# this is the path automatically set by vscode extension
set(PICO_SDK_PATH "${USERHOME}/.pico-sdk/sdk/2.1.1")

# initialize the SDK based on PICO_SDK_PATH
# this must be done before project()
include(pico_sdk_import.cmake)


project ("pico-cmake-cpp" C CXX ASM)
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

# add_subdirectory(labs)
# add_subdirectory(assignments)
# add_subdirectory(examples)
# add_subdirectory(tools/debugprobe)