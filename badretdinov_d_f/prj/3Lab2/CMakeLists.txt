cmake_minimum_required(VERSION 3.0)

project(3Lab2)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(3Lab2 3Lab2.cpp)
target_link_libraries(3Lab2 ${OpenCV_LIBS})