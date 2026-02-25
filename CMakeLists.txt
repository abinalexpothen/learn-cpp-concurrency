cmake_minimum_required(VERSION 3.15)
project(learn_cpp_concurrency VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_subdirectory(learn/thread-management)

add_subdirectory(tracy)

# 2. Build the Profiler GUI (The actual app you look at)
# We check if we are on Linux/Unix to build the profiler
if(UNIX)
    # 1. Keep your working X11/Legacy flags
    set(LEGACY ON CACHE BOOL "" FORCE)
    set(TRACY_NO_WAYLAND ON CACHE BOOL "" FORCE)

    # 2. Fix the TBB Linker Error
    # We find the TBB package and force the profiler to link it
    find_package(TBB REQUIRED)
    
    add_subdirectory(tracy/profiler)
    
    # This line tells the 'tracy-profiler' target to link against TBB
    target_link_libraries(tracy-profiler PRIVATE TBB::tbb)
endif()

# Create a toggle so you can turn profiling off without changing code
option(TRACY_ENABLE "Enable Tracy profiling" ON)

if(TRACY_ENABLE)
    add_compile_definitions(TRACY_ENABLE)
endif()

