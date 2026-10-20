cmake_minimum_required(VERSION 3.16)
project(armor_demo)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(armor)

add_executable(armor_main main.cpp)
target_link_libraries(armor_main armor_lib ${OpenCV_LIBS})
