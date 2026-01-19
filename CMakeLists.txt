cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico2 CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(donutmelt C CXX ASM)

pico_sdk_init()

add_executable(donutmelt src/donutmelt.c)
add_subdirectory(lib/pico_H3LIS331DL)
add_subdirectory(lib/c_wrapped_pico_dshot)
#add_subdirectory(lib/pico_crsf) 

pico_enable_stdio_uart(donutmelt 0)
pico_enable_stdio_usb(donutmelt 1)

target_link_libraries(donutmelt 
    c_wrapped_pico_dshot
    pico_H3LIS331DL
    pico_stdlib
    hardware_i2c
    hardware_uart
)

pico_add_extra_outputs(donutmelt)