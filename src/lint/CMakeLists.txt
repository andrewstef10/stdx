# Separate OBJECT library so all_headers.cpp is compiled (and therefore reachable by clang-tidy) without being archived into the stdx .lib
add_library(${PROJECT_NAME}_lint OBJECT all_headers.cpp)
target_include_directories(${PROJECT_NAME}_lint PRIVATE ${CMAKE_SOURCE_DIR}/include)
check_enable_clang_tidy(${PROJECT_NAME}_lint)
