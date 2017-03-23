cmake_minimum_required(VERSION 3.0)

project(colorCorrection2)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(colorCorrection2 colorCorrection2.cpp)
target_link_libraries(colorCorrection2 ${OpenCV_LIBS})