## Setup minimum required version for CMake
cmake_minimum_required(VERSION 3.22)

project(AlarmSys VERSION 1.0
LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(AlarmSys problem.cpp problem.h)

target_include_directories (AlarmSys PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(alarm_sys main.cpp)
target_link_libraries(alarm_sys PRIVATE AlarmSys)
