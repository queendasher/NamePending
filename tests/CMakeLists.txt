# tests/CMakeLists.txt
# -------------------------------------------------------------
# Catch2 setup
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

include(Catch)  # provides catch_discover_tests()
include(CTest)

# -------------------------------------------------------------
# Aggregate target for all test executables
if(NOT TARGET tests-all)
  add_custom_target(tests-all)
endif()

# -------------------------------------------------------------
# Common helper to create Catch2-based test executables
function(_add_catch_exe exe label)
  add_executable(${exe} ${ARGN})
  target_link_libraries(${exe} PRIVATE Catch2::Catch2WithMain mathlib)

  # Place binaries in <build>/tests for all configurations
  set_target_properties(${exe} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                 ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_DEBUG           ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_RELEASE         ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO  ${CMAKE_BINARY_DIR}/tests
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL      ${CMAKE_BINARY_DIR}/tests
  )

  # Add each test exe to the aggregate build target
  add_dependencies(tests-all ${exe})
endfunction()

# -------------------------------------------------------------
# Wrapper for normal unit tests
function(add_catch_unit_test exe)
  _add_catch_exe(${exe} "unit" ${ARGN})

  catch_discover_tests(${exe}
    TEST_PREFIX       "${exe}."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    PROPERTIES        LABELS "unit"
  )
endfunction()

# -------------------------------------------------------------
# Wrapper for performance / benchmark tests
function(add_catch_perf_test exe)
  _add_catch_exe(${exe} "perf" ${ARGN})

  catch_discover_tests(${exe}
    TEST_PREFIX       "${exe}."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    PROPERTIES        LABELS "perf"
    EXTRA_ARGS
      --benchmark-samples 50
      --benchmark-warmup-time 200ms
      #--benchmark-confidence-interval 0.95
      #--benchmark-resamples 100
  )
endfunction()

# -------------------------------------------------------------
# Automatically discover and register tests
file(GLOB UNIT_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/test_*.cpp)
foreach(src IN LISTS UNIT_SOURCES)
  get_filename_component(name "${src}" NAME_WE)
  add_catch_unit_test(${name} "${src}")
endforeach()

file(GLOB PERF_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/perf_*.cpp)
foreach(src IN LISTS PERF_SOURCES)
  get_filename_component(name "${src}" NAME_WE)
  add_catch_perf_test(${name} "${src}")
endforeach()
