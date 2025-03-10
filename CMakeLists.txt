cmake_minimum_required(VERSION 3.20.0)
project(path-tracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(${PROJECT_NAME} main.cpp)
