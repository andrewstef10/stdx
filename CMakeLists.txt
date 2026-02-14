cmake_minimum_required(VERSION 3.20)
project(stdads LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ---- Library ----
add_library(stdads
    src/math.cpp
    src/string.cpp
    src/Stopwatch.cpp
)

target_include_directories(stdads
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# ---- GoogleTest ----
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/85087857ad10bd407cd6ed2f52f7ea9752db621f.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# ---- Tests ----
add_executable(stdads_tests
    tests/test_math.cpp
    tests/test_string.cpp
    tests/TestStopwatch.cpp
)

target_link_libraries(stdads_tests
    stdads
    gtest
    gtest_main
)

enable_testing()
add_test(NAME stdads_tests COMMAND stdads_tests)
