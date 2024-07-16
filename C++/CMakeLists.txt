cmake_minimum_required(VERSION 3.0)
project(MULTITHREADING)


find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(MULTITHREADING greyscale_multithreading.cpp)

target_link_libraries(MULTITHREADING ${OpenCV_LIBS})