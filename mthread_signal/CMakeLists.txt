cmake_minimum_required(VERSION 2.8)
project(test-cmake-project)

set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g" )

add_executable(test_dk main.cpp)
TARGET_LINK_LIBRARIES(test_dk pthread)
