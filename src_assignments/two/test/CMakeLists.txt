project(two_test)

enable_testing()

# test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(test_HEADERS
)

set(test_SOURCES
        Vec2_test.cpp)

add_executable(two_test ${test_SOURCES} ${test_HEADERS} ${mock_HEADERS})

target_include_directories(two_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})

target_link_libraries(two_test PRIVATE src_lib gtest_main gmock pthread)