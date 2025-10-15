cmake_minimum_required(VERSION 3.20)
project(NamePending LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories for build targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Header-only library from files in ./src
add_library(mathlib INTERFACE)
target_include_directories(mathlib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Demo executable
add_executable(demo demos/demo.cpp)
target_link_libraries(demo PRIVATE mathlib)

enable_testing()
add_subdirectory(tests EXCLUDE_FROM_ALL)