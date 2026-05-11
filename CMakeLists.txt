cmake_minimum_required(VERSION 3.20)
project(stdx LANGUAGES CXX)

# ---- Global Settings ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # exports compile_commands.json to build directory

# ---- Options ----
option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

# ---- Output Directories ----
set(COVERAGE_DIR ${CMAKE_SOURCE_DIR}/coverage)

message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}") # This is the root of the source tree (where root CMakeLists.txt is located)
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}") # This is the directory of the current CMakeLists.txt
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}") # This is the root of the build tree (where you run cmake. e.g. build/)
message(STATUS "COVERAGE_DIR = ${COVERAGE_DIR}")


# ============================================================
# Compiler Flags / properties functions
# ============================================================
function(check_enable_clang_tidy target)
    if(ENABLE_CLANG_TIDY)
        find_program(CLANG_TIDY_EXE NAMES clang-tidy)

        if(CLANG_TIDY_EXE)
            message(STATUS "Found clang-tidy found at: ${CLANG_TIDY_EXE}")

            set_target_properties(stdx PROPERTIES
                CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
            )
        else()
            message(WARNING "clang-tidy enabled but not found!")
        endif()
    endif()
endfunction()

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
add_library(stdx
    src/ctype.cpp
    src/math.cpp
    src/stdxlib.cpp
    src/stopwatch.cpp
    src/string.cpp
)

target_include_directories(stdx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

check_enable_clang_tidy(stdx)
check_enable_sanitizers(stdx)
check_enable_coverage(stdx)


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

add_executable(stdx_tests
    tests/test_array.cpp
    tests/test_ctype.cpp
    tests/test_iterator.cpp
    tests/test_math.cpp
    tests/test_num_utils.cpp
    tests/test_stdxlib.cpp
    tests/test_stopwatch.cpp
    tests/test_string.cpp
)

target_link_libraries(stdx_tests
    PRIVATE
        stdx
        gtest
        gtest_main
)

check_enable_sanitizers(stdx_tests)
check_enable_coverage(stdx_tests)

add_test(NAME stdx_tests COMMAND stdx_tests)


# ============================================================
# Custom Targets
# ============================================================
if(ENABLE_COVERAGE)
    find_program(GCOVR_PATH gcovr)

    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "gcovr enabled but not found! Install it with: sudo apt install gcovr")
    endif()

    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}
        COMMAND ${GCOVR_PATH}
            -r ${CMAKE_SOURCE_DIR}
            --filter ${CMAKE_SOURCE_DIR}/src/
            --filter ${CMAKE_SOURCE_DIR}/include/
            --exclude-throw-branches
            --exclude-unreachable-branches
            --html
            --html-details
            --html-self-contained
            --html-title "stdx Coverage Report"
            --html-details=${COVERAGE_DIR}/index.html
            --txt=${COVERAGE_DIR}/coverage.txt
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