cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# message(STATUS "The Cmake file you want to be running is running")

add_subdirectory(pico-bidir-dshot)

add_library(c_wrapped_pico_dshot
    c_pico_dshot.cpp
)

target_include_directories(c_wrapped_pico_dshot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# The line below this comment shouldn't be necessary because of what the CMake file in pico-bidir-dshot looks like
# target_include_directories(c_wrapped_pico_dshot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pico-bidir-dshot/src) # gives access to library header files just in case

target_link_libraries(c_wrapped_pico_dshot
    Pico_Bidir_DShot
    hardware_pio
)