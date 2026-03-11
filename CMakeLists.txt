cmake_minimum_required(VERSION 3.20)
project(stdads LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Coverage Option ----
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "Coverage enabled")

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O0 -g --coverage)
        add_link_options(--coverage)
    else()
        message(WARNING "Coverage only supported with GCC/Clang")
    endif()
endif()


# ---- Library ----
add_library(stdads
    src/Ctype.cpp
    src/math.cpp
    src/Stdadslib.cpp
    src/Stopwatch.cpp
    src/String.cpp
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
    tests/TestCtype.cpp
    tests/test_math.cpp
    tests/TestNumUtils.cpp
    tests/TestStdadslib.cpp
    tests/TestStopwatch.cpp
    tests/TestString.cpp
)

target_link_libraries(stdads_tests
    stdads
    gtest
    gtest_main
)

enable_testing()
add_test(NAME stdads_tests COMMAND stdads_tests)


# ---- Coverage Target ----
if(ENABLE_COVERAGE)

    find_program(GCOVR_PATH gcovr)

    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "gcovr not found! Install it with: sudo apt install gcovr")
    endif()

    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory coverage
        COMMAND ${GCOVR_PATH}
            -r ${CMAKE_SOURCE_DIR}
            --exclude ${CMAKE_BINARY_DIR}
            --html
            --html-details
            -o coverage/index.html
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating coverage report..."
    )

endif()