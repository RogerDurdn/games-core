cmake_minimum_required(VERSION 3.26)
project(lectures)

# system
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# executables
add_executable(lecture2 lecture2.cpp)
add_executable(lecture3 lecture3.cpp)
add_executable(lecture4 lecture4.cpp)

# libs
include_directories(${SFML_INCLUDE_DIRS})

# sfml
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(lecture4 sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
