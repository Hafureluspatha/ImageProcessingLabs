cmake_minimum_required(VERSION 3.0)

project(graph)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(graph graph.cpp)
target_link_libraries(graph ${OpenCV_LIBS})