cmake_minimum_required(VERSION 3.0)

project(2controlWork1)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(2controlWork1 2controlWork1.cpp)
target_link_libraries(2controlWork1 ${OpenCV_LIBS})