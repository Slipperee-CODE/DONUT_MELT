cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# message(STATUS "The Cmake file you want to be running is running")

add_subdirectory(pico-bidir-dshot)

add_library(c_pico_bidir_dshot
    c_pico_dshot.cpp
)

target_include_directories(c_pico_bidir_dshot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(c_pico_bidir_dshot
    Pico_Bidir_DShot
    hardware_pio
)