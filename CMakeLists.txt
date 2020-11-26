cmake_minimum_required(VERSION 2.8)
project(img_processor)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_executable(img_processor img_processor.cpp)
target_link_libraries(img_processor ${OpenCV_LIBS})
