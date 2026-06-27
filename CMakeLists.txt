cmake_minimum_required(VERSION 3.20)
project(stdx LANGUAGES CXX)
include(GNUInstallDirs)

# ---- Global Settings ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DEBUG_POSTFIX "-d") # postfix all libraries for debug builds

# ---- Options ----
option(ENABLE_UNIT_TESTS "Enable building unit tests" ON)
option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

# ---- Output Directories ----
set(COVERAGE_DIR ${CMAKE_SOURCE_DIR}/coverage)

message(STATUS "----General CMake Variables----")
message(STATUS "CMAKE_PROJECT_NAME              = ${CMAKE_PROJECT_NAME}")
message(STATUS "CMAKE_SOURCE_DIR                = ${CMAKE_SOURCE_DIR}") # This is the root of the source tree (where root CMakeLists.txt is located)
message(STATUS "CMAKE_CURRENT_SOURCE_DIR        = ${CMAKE_CURRENT_SOURCE_DIR}") # This is the directory of the current CMakeLists.txt
message(STATUS "CMAKE_BINARY_DIR                = ${CMAKE_BINARY_DIR}") # This is the root of the build tree (where you run cmake. e.g. build/)

message(STATUS "----Project Install Locations----")
message(STATUS "CMAKE_INSTALL_PREFIX            = ${CMAKE_INSTALL_PREFIX}") # where running cmake --install will install to
message(STATUS "CMAKE_INSTALL_BINDIR            = ${CMAKE_INSTALL_BINDIR}") # subdirectory where all executables should be installed to
message(STATUS "CMAKE_INSTALL_LIBDIR            = ${CMAKE_INSTALL_LIBDIR}") # subdirectory where all library files should be installed to
message(STATUS "CMAKE_INSTALL_INCLUDEDIR        = ${CMAKE_INSTALL_INCLUDEDIR}") # subdirectory where all header files should be installed to

message(STATUS "----Project Build Output Locations----")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY  = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}") # where executables will be output after building
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY  = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}") # where static library files will be output after building
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY  = ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}") # where shared library files will be output after building

message(STATUS "----Architecture----")
message(STATUS "CMAKE_SYSTEM_NAME            = ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_VERSION         = ${CMAKE_SYSTEM_VERSION}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR       = ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_GENERATOR              = ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_C_COMPILER             = ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER           = ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_C_COMPILER_ID          = ${CMAKE_C_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_ID        = ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_C_COMPILER_VERSION     = ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION   = ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_SIZEOF_VOID_P          = ${CMAKE_SIZEOF_VOID_P}")
message(STATUS "CMAKE_BUILD_TYPE             = ${CMAKE_BUILD_TYPE}")

message(STATUS "----Other----")
message(STATUS "COVERAGE_DIR                    = ${COVERAGE_DIR}")


# ============================================================
# Compiler Flags / properties functions for Targets
# ============================================================
function(enable_compiler_warnings target)
    if(MSVC)
        # Covers cl.exe and clang-cl (Clang targeting the MSVC ABI)
        target_compile_options(${target} PRIVATE
            $<$<COMPILE_LANGUAGE:C>:/W4>
            $<$<COMPILE_LANGUAGE:CXX>:/W4>
        )
    else()
        target_compile_options(${target} PRIVATE
            $<$<COMPILE_LANG_AND_ID:C,GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
            $<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
        )
    endif()
endfunction()

function(check_enable_clang_tidy target)
    if(ENABLE_CLANG_TIDY)
        find_program(CLANG_TIDY_EXE NAMES clang-tidy)

        if(CLANG_TIDY_EXE)
            message(STATUS "Found clang-tidy found at: ${CLANG_TIDY_EXE}")

            set_target_properties(${target} PROPERTIES
                CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
            )
        else()
            message(WARNING "clang-tidy enabled but not found! Ensure clang-tidy is installed!")
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
add_subdirectory(src)


# ============================================================
# Tests
# ============================================================
if(ENABLE_UNIT_TESTS)
    enable_testing()
    add_subdirectory(tests)
else()
    message(STATUS "Building for unit tests disabled")
endif()


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
            --html-title "${CMAKE_PROJECT_NAME} Coverage Report"
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