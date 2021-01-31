cmake_minimum_required(VERSION 2.8)
project(test-cmake-project)

set(CMAKE_CXX_STANDARD 17)
set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g" )

add_executable(test main.cpp)
TARGET_LINK_LIBRARIES(test pthread)
