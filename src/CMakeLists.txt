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

if(ENABLE_CLANG_TIDY)
    # Separate OBJECT library so all_headers.cpp is compiled (and therefore reachable by clang-tidy) without being archived into the stdx .lib
    add_library(${PROJECT_NAME}_lint OBJECT all_headers.cpp)
    target_include_directories(${PROJECT_NAME}_lint PRIVATE ${CMAKE_SOURCE_DIR}/include)
    check_enable_clang_tidy(${PROJECT_NAME}_lint)
endif()


install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)