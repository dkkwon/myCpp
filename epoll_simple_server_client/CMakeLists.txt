cmake_minimum_required(VERSION 2.8)
project(test-cmake-project)

set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g" )

add_executable(server server.cpp)
TARGET_LINK_LIBRARIES(server pthread)

add_executable(client client.cpp)

