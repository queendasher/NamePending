# tests/CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

include(Catch)  # provides catch_discover_tests

# Ensure we have one aggregate target to build all test executables
if(NOT TARGET tests-all)
  add_custom_target(tests-all)  # empty target collecting all tests
endif()

function(add_catch_test exe)
  # Remaining args are the sources for this test exe
  add_executable(${exe} ${ARGN})
  target_link_libraries(${exe} PRIVATE Catch2::Catch2WithMain mathlib)

  # Put test binaries under <build>/tests for all common configs
  set_target_properties(${exe} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                 ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_DEBUG           ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_RELEASE         ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO  ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL      ${CMAKE_BINARY_DIR}/tests
  )

  # Register tests with CTest.
  catch_discover_tests(${exe}
    TEST_PREFIX       "${exe}."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    PROPERTIES        LABELS "unit"
  )

  # Make 'tests-all' depend on this exe so we can build all tests at once
  add_dependencies(tests-all ${exe})
endfunction()

# ---- Collect all test sources and add them to the target ----
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/test_*.cpp)
set(TEST_BINS "")
foreach(src IN LISTS TEST_SOURCES)
  get_filename_component(name "${src}" NAME_WE)
  add_catch_test(${name} "${src}")
  list(APPEND TEST_BINS ${name})
endforeach()

add_custom_target(build-tests DEPENDS ${TEST_BINS})
