cmake_minimum_required(VERSION 3.10)
project(PriorityQueue)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_executable(priority_queue src/main.cpp)

# Optional: build tests executable with Catch2 single-header in catch.hpp
add_executable(priority_queue_tests
    src/tests/tests.cpp
)
