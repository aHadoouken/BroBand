cmake_minimum_required(VERSION 3.18)

project(server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -pthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -pthread")

add_compile_options("--coverage")
add_link_options("--coverage")

include_directories(src)

add_subdirectory(src)
add_executable(main.out main.cpp)
target_link_libraries(main.out server_lib)

#enable_testing()
#find_package(GTest REQUIRED)
#
#include_directories(${GTEST_INCLUDE_DIRS})
#
#add_executable(tests.out tests/tests.cpp)
#target_link_libraries(tests.out ${GTEST_LIBRARIES} db_lib gcov)

