cmake_minimum_required(VERSION 3.26)
project(four)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

include_directories(${SFML_INCLUDE_DIRS})
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)


file(GLOB notLink CONFIGURE_DEPENDS "*.h" "*.cpp")
add_executable(links ${notLink})
target_link_libraries(links sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
