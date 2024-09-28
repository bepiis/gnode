cmake_minimum_required(VERSION 3.30)
project(GNode CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Catch2 3 REQUIRED)
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.7.1
)

# find_package(Python3 REQUIRED COMPONENTS Interpreter Development)


FetchContent_MakeAvailable(Catch2)
# These tests can use the Catch2-provided main

add_executable(test tests/test_main.cpp matrix.h)

# target_include_directories(test PRIVATE ${Python3_INCLUDE_DIRS})
# target_link_libraries(test PRIVATE Catch2::Catch2WithMain ${Python3_LIBRARIES})
target_link_libraries(test PRIVATE Catch2::Catch2WithMain)
