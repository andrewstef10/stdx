add_library(${PROJECT_NAME}
    ctype.cpp
    math.cpp
    stdxlib.cpp
    stopwatch.cpp
    string.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

enable_compiler_warnings(${PROJECT_NAME})
check_enable_clang_tidy(${PROJECT_NAME})
check_enable_sanitizers(${PROJECT_NAME})
check_enable_coverage(${PROJECT_NAME})

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(ENABLE_CLANG_TIDY)
    add_subdirectory(lint)
endif()
