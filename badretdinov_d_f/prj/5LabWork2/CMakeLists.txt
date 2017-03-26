cmake_minimum_required(VERSION 3.0)

project(LabWork2)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(LabWork2 LabWork2.cpp)
target_link_libraries(LabWork2 ${OpenCV_LIBS})