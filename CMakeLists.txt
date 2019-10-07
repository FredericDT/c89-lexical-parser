project(lexc89)
set(VERSION 0.1)
cmake_minimum_required(VERSION 3.10)

add_subdirectory(lib/cxxopts)
add_subdirectory(lib/re2)
add_library(lexc89 INTERFACE)

target_include_directories(lexc89 INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(test)
add_subdirectory(src)