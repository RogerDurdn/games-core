cmake_minimum_required(VERSION 3.26)
project(core-games)
add_subdirectory(src_assignments/two)

# system
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# libs
include_directories(${SFML_INCLUDE_DIRS})
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

set(targets
        loading
        lecture2
        lecture3
        lecture4
        one
)

set(files
        main.cpp
        src_lectures/lecture2.cpp
        src_lectures/lecture3.cpp
        src_lectures/lecture4.cpp
        src_assignments/one/one.cpp
)

foreach(target file IN ZIP_LISTS targets files)
    add_executable(${target} ${file} )
    target_link_libraries(${target} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
endforeach()
