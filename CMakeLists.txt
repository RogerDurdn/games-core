cmake_minimum_required(VERSION 3.26)
project(core-games)

# system
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# libs
include_directories(${SFML_INCLUDE_DIRS})
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

set(runners
        lecture2
        lecture3
        lecture4
        one
)

set(targets
        src_lectures/lecture2.cpp
        src_lectures/lecture3.cpp
        src_lectures/lecture4.cpp
        src_assignments/one/one.cpp
)

foreach(runner target IN ZIP_LISTS runners targets)
    add_executable(${runner} ${target} )
    target_link_libraries(${runner} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
endforeach()
