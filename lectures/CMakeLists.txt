cmake_minimum_required(VERSION 3.26)
project(lectures)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(lecture2 lecture2.cpp)
add_executable(lecture3 lecture3.cpp)
