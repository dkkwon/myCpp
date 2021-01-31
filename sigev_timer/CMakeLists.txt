cmake_minimum_required(VERSION 2.8)
project(test-cmake-project)

set(CMAKE_CXX_STANDARD 17)
set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g" )

# ADD_EXECUTABLE(test main_ref.cpp)
ADD_EXECUTABLE(test main.cpp sigtimer.cpp sigtimerhandler.cpp)
TARGET_LINK_LIBRARIES(test rt pthread)
