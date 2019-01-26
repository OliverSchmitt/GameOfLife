cmake_minimum_required(VERSION 3.12)
project(GameOfLive)

set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(GameOfLive main.cpp GameOfLive.cpp GameOfLive.h)
target_link_libraries(GameOfLive sfml-graphics sfml-window sfml-system)