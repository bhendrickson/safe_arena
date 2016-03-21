cmake_minimum_required(VERSION 2.8.7)

project(safe_arena)

include_directories(
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_BINARY_DIR}/gsl-prefix/src/gsl/include/
)

include(ExternalProject)

ExternalProject_Add(gsl
    URL https://github.com/Microsoft/GSL/archive/ad0905c08ba7ef170f3ce8b807ca822d7db108ac.tar.gz
    URL_HASH SHA256=b9e3cc5cf0c69b63705063bbe26fe43a58d6986de64338e2e4374bd9eb3a0241
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
endif()

add_executable(benchmark_arena benchmark_arena.cc)
