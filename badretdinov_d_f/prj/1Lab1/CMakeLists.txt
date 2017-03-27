cmake_minimum_required(VERSION 3.0)

project(1Lab1)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(1Lab1 1Lab1.cpp)
target_link_libraries(1Lab1 ${OpenCV_LIBS})