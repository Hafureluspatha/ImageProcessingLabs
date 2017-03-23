cmake_minimum_required(VERSION 3.0)

project(controlWork1)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(controlWork1 controlWork1.cpp)
target_link_libraries(controlWork1 ${OpenCV_LIBS})