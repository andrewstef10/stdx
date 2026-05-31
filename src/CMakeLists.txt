add_library(${CMAKE_PROJECT_NAME}
    ctype.cpp
    math.cpp
    stdxlib.cpp
    stopwatch.cpp
    string.cpp
)


target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)


if (MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()


check_enable_clang_tidy(${CMAKE_PROJECT_NAME})
check_enable_sanitizers(${CMAKE_PROJECT_NAME})
check_enable_coverage(${CMAKE_PROJECT_NAME})


install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)