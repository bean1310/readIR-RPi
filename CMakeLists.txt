cmake_minimum_required( VERSION 3.0 )

project(readIR-RPi C)

set(CMAKE_C_FLAGS "-Wall")

add_executable(readIR-RPi
        src/readIR.c)

find_library(wiringPi_LIB wiringPi)
target_link_libraries(readIR-RPi ${wiringPi_LIB})

