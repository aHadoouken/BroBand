cmake_minimum_required(VERSION 3.16)

project(server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -pthread")

add_compile_options("--coverage")
add_link_options("--coverage")

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(main.out main.cpp)
target_link_libraries(main.out ${Boost_LIBRARIES} Threads::Threads server_lib)
