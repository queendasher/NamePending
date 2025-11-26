# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELWITHDEBINFO CACHE STRING "Choose the type of build." FORCE)
endif()

include_directories(../src ../NamePending-HPC/concurrentqueue)

add_executable (demo demo.cpp
                ../NamePending-HPC/src/taskmanager.cpp ../NamePending-HPC/src/timer.cpp)
                
target_sources (demo PUBLIC ../NamePending-HPC/src/taskmanager.hpp ../NamePending-HPC/src/timer.hpp)