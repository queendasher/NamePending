cmake_minimum_required(VERSION 3.20)
project(NamePending LANGUAGES CXX)

# C++ settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Put build artifacts under the build tree in nice folders
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # .a / .lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   # .so / .dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)   # executables

# .cpp implementations
add_library(mathlib STATIC
  src/complex.cpp
  src/polynomial.cpp
)

# Public include path for mathlib
target_include_directories(mathlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)

# Build executable that links with the library
add_executable(demo demos/demo.cpp)
target_link_libraries(demo PRIVATE mathlib)
