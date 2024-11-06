cmake_minimum_required(VERSION 3.30)
project(linalg_core CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB_RECURSE LINALG_CORE_INCLUDES include/*.h)

add_library(${PROJECT_NAME} INTERFACE)
add_library(linalg::core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/include})

option(TEST_LINALG_CORE "TEST" ON)
if(${TEST_LINALG_CORE})
    Include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1
    )

    FetchContent_MakeAvailable(Catch2)

    add_executable(test tests/test_main.cpp)

    target_link_libraries(test PRIVATE Catch2::Catch2WithMain linalg::core)

endif()