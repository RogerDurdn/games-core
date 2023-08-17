cmake_minimum_required(VERSION 3.26)
project(second_assignment)
add_subdirectory(test)

add_library(src_lib
        Vec2.cpp
)

# system
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# libs
include_directories(${SFML_INCLUDE_DIRS})
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

add_executable(shape_game main.cpp
        Vec2.cpp
        Game.cpp
        Components.h
        Entity.cpp)
target_link_libraries(shape_game sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
