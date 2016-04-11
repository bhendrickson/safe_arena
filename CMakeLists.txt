cmake_minimum_required(VERSION 2.8.7)

project(safe_arena)

include_directories(
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_BINARY_DIR}/gsl-prefix/src/gsl/include/
)

include(ExternalProject)

ExternalProject_Add(gsl
    URL https://github.com/Microsoft/GSL/archive/a9f865900d28b854de5ead971aadb82e5ef9ed40.zip
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
endif()

add_executable(benchmark_arena benchmark_arena.cc)
