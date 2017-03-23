cmake_minimum_required(VERSION 3.0)

project(colorCorrection1)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(colorCorrection1 colorCorrection1.cpp)
target_link_libraries(colorCorrection1 ${OpenCV_LIBS})