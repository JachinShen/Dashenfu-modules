cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(dashenfu)

find_package(OpenCV 2.4.8 REQUIRED)

add_executable (dashenfu dashenfu.cpp)
target_link_libraries (dashenfu ${OpenCV_LIBS})

