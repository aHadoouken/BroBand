cmake_minimum_required(VERSION 3.18)

project(DataBase)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -pthread")

add_compile_options("--coverage")
add_link_options("--coverage")

include_directories(src)

add_subdirectory(src)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

add_executable(main.out main.cpp)
target_link_libraries(main.out ${Boost_LIBRARIES} db_lib ${PQXX_LIB} ${PQ_LIB})

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests.out tests/tests.cpp)
target_link_libraries(tests.out ${GTEST_LIBRARIES} ${Boost_LIBRARIES} db_lib ${PQXX_LIB} ${PQ_LIB} gcov)

