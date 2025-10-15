include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0  # Use the latest stable version
)

FetchContent_MakeAvailable(Catch2)


add_executable(tests_vector tests_vector.cpp)
target_sources(tests_vector PRIVATE ../src/vector.hpp ../src/expression.hpp)
target_link_libraries(tests_vector PRIVATE Catch2::Catch2WithMain)

# Put the test exe in <build>/tests for all common configs
set_target_properties(tests_vector PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)
foreach(cfg Debug Release RelWithDebInfo MinSizeRel)
  set_target_properties(tests_vector PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_${cfg} ${CMAKE_BINARY_DIR}/tests
  )
endforeach()

add_test(NAME catchtest COMMAND tests_vector)


