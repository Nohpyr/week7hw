cmake_minimum_required(VERSION 2.8)

project(distance)

add_executable(distance main.cpp)

find_package(OpenCV REQUIRED)
target_link_libraries(distance ${OpenCV_LIBS})
