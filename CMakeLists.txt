cmake_minimum_required(VERSION 3.30)
project(GNode CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 3 REQUIRED)
# These tests can use the Catch2-provided main
add_executable(test tests/test_main.cpp matrix.hpp)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)
