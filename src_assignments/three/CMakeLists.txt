cmake_minimum_required(VERSION 3.26)
project(three)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

include_directories(${SFML_INCLUDE_DIRS})
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)


file(GLOB megaManSrc CONFIGURE_DEPENDS "*.h" "*.cpp")
add_executable(mega ${megaManSrc})
target_link_libraries(mega sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
