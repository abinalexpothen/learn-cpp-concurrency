cmake_minimum_required(VERSION 3.15)
project(learn_cpp_concurrency VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_executable(learn-cpp-concurrency hello.cpp)

target_compile_options(learn-cpp-concurrency PRIVATE
  $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic>
  $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic>
)

target_include_directories(learn-cpp-concurrency PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(learn/thread-management)

