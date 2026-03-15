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

project(c_pico_bidir_dshot C CXX ASM)

pico_sdk_init()

add_subdirectory(pico-bidir-dshot)

add_library(c_pico_bidir_dshot
    c_pico_dshot.cpp
)

target_include_directories(c_pico_bidir_dshot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(c_pico_bidir_dshot
    Pico_Bidir_DShot
    hardware_pio
)

if (PROJECT_IS_TOP_LEVEL)
    add_executable(default_test tests/default_test.c)

    pico_enable_stdio_uart(default_test 0)
    pico_enable_stdio_usb(default_test 1)

    target_link_libraries(default_test
        pico_stdio
        pico_stdlib
        c_pico_bidir_dshot
    )

    pico_add_extra_outputs(default_test) 
endif()