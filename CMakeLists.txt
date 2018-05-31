cmake_minimum_required(VERSION 3.10)
project(MN_Interpolation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")
add_executable(MN_Interpolation main.cpp string_tools.hh matrix.hh exceptions.hh iostream_tools.hh linear_system.hh)