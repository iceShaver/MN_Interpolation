cmake_minimum_required(VERSION 3.10)
project(MN_Interpolation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread -O3")
add_executable(MN_Interpolation main.cc string_tools.hh interpolation.hh interpolation.cc)