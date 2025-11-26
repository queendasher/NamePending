# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELWITHDEBINFO CACHE STRING "Choose the type of build." FORCE)
endif()

if(WIN32)
   file (DOWNLOAD https://sourceforge.net/projects/openblas/files/v0.3.24/OpenBLAS-0.3.24-x64.zip/download "${CMAKE_BINARY_DIR}/openblas.zip" EXPECTED_MD5 fc08fe8c0dc7364da115d0e09b5a134f)
   file (ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/openblas.zip DESTINATION ${CMAKE_BINARY_DIR}/openblas)
   set(BLAS_LIBRARIES ${CMAKE_BINARY_DIR}/openblas/lib/libopenblas.dll.a CACHE PATH "BLAS" FORCE)
   set(LAPACK_LIBRARIES ${CMAKE_BINARY_DIR}/openblas/lib/libopenblas.dll.a CACHE PATH "LAPACK" FORCE)
endif()

find_package(LAPACK REQUIRED)

include_directories(../src ../NamePending-HPC/concurrentqueue)

add_executable (demo demo.cpp
                ../NamePending-HPC/src/taskmanager.cpp ../NamePending-HPC/src/timer.cpp)
target_link_libraries (demo PUBLIC LAPACK::LAPACK)
                
target_sources (demo PUBLIC ../NamePending-HPC/src/taskmanager.hpp ../NamePending-HPC/src/timer.hpp)

if(WIN32)
    add_custom_command(TARGET test_lapack POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_BINARY_DIR}/openblas/bin/libopenblas.dll"
      $<TARGET_FILE_DIR:test_lapack>
    )
endif()