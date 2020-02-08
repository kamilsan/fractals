cmake_minimum_required(VERSION 2.4)
project(Fractals)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 14)

include_directories(common)

add_executable(fern 
  barnsleys_fern/main.cpp
  common/color.hpp common/color.cpp
  common/image.hpp common/image.cpp)

add_executable(buddhabrot 
  buddhabrot/main.cpp
  common/color.hpp common/color.cpp
  common/image.hpp common/image.cpp)

add_executable(burning_ship 
  burning_ship/main.cpp
  common/color.hpp common/color.cpp
  common/image.hpp common/image.cpp)

add_executable(h_tree 
  h_tree/main.cpp
  common/color.hpp common/color.cpp
  common/image.hpp common/image.cpp)

add_executable(t_squere 
  t_squere/main.cpp
  common/color.hpp common/color.cpp
  common/image.hpp common/image.cpp)

add_executable(mandelbrot 
  mandelbrot/main.cpp
  common/color.hpp common/color.cpp
  common/image.hpp common/image.cpp)