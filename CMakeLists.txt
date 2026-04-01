cmake_minimum_required(VERSION 3.20)
project(stdads LANGUAGES CXX)

# ---- Global Settings ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Options ----
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

# ---- Output Directories ----
set(COVERAGE_DIR ${CMAKE_SOURCE_DIR}/coverage)

message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}") # This is the root of the source tree (where root CMakeLists.txt is located)
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}") # This is the directory of the current CMakeLists.txt
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}") # This is the root of the build tree (where you run cmake. e.g. build/)
message(STATUS "COVERAGE_DIR = ${COVERAGE_DIR}")


# ============================================================
# Compiler Flags functions
# ============================================================
function(check_enable_sanitizers target)
    if(ENABLE_ASAN)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            message(STATUS "AddressSanitizer (Asan) enabled for target: ${target}")
            target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer -g)
            target_link_options(${target} PRIVATE -fsanitize=address)
        else()
            message(WARNING "AddressSanitizer (Asan) only supported with GCC/Clang. ASan not enabled for target: ${target}")
        endif()
    endif()
endfunction()

function(check_enable_coverage target)
    if(ENABLE_COVERAGE)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            message(STATUS "Coverage enabled for target: ${target}")
            target_compile_options(${target} PRIVATE --coverage -O0 -g)
            target_link_options(${target} PRIVATE --coverage)
        else()
            message(WARNING "Coverage only supported with GCC/Clang. Coverage not enabled for target: ${target}")
        endif()
    endif()
endfunction()


# ============================================================
# Library
# ============================================================
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

check_enable_sanitizers(stdads)
check_enable_coverage(stdads)


# ============================================================
# Dependencies
# ============================================================
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/85087857ad10bd407cd6ed2f52f7ea9752db621f.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# ============================================================
# Tests
# ============================================================
enable_testing()

add_executable(stdads_tests
    tests/TestCtype.cpp
    tests/test_math.cpp
    tests/TestNumUtils.cpp
    tests/TestStdadslib.cpp
    tests/TestStopwatch.cpp
    tests/TestString.cpp
)

target_link_libraries(stdads_tests
    PRIVATE
        stdads
        gtest
        gtest_main
)

check_enable_sanitizers(stdads_tests)
check_enable_coverage(stdads_tests)

add_test(NAME stdads_tests COMMAND stdads_tests)


# ============================================================
# Custom Targets
# ============================================================
if(ENABLE_COVERAGE)
    find_program(GCOVR_PATH gcovr)

    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "gcovr not found! Install it with: sudo apt install gcovr")
    endif()

    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}
        COMMAND ${GCOVR_PATH}
            -r ${CMAKE_SOURCE_DIR}
            --filter ${CMAKE_SOURCE_DIR}/src
            --filter ${CMAKE_SOURCE_DIR}/include
            --html
            --html-details
            --html-self-contained
            --html-title "stdads Coverage Report"
            -o ${COVERAGE_DIR}/index.html
            --print-summary
        COMMENT "Generating coverage report..."
    )
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile # Documentation will be generated to the docs\html directory
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating Doxygen documentation..."
    )
else()
    message(STATUS "Doxygen not found. Doxygen target (--target doxygen) will not be available. Install Doxygen to enable documentation generation.")
endif()