cmake_minimum_required(VERSION 3.16)

project(server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -pthread")

add_compile_options("--coverage")
add_link_options("--coverage")

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)

add_executable(main.out main.cpp)
target_link_libraries(main.out server_lib)
