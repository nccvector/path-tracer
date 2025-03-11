cmake_minimum_required(VERSION 3.20.0)
project(path-tracer LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 11.1.4
)
FetchContent_MakeAvailable(fmt)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} fmt::fmt)
