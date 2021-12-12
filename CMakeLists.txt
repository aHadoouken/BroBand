cmake_minimum_required(VERSION 3.16)

project(server_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -pthread")


add_compile_options("--coverage")
add_link_options("--coverage")

include_directories(server/src)
include_directories(server/tests)

list(APPEND CMAKE_PREFIX_PATH "/home/alex/detecting/libtorch")
find_package(Torch REQUIRED)
include_directories(${TORCH_INCLUDE_DIRS})
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_subdirectory(server/src)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

add_executable(main.out server/main.cpp)
target_link_libraries(main.out ${TORCH_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES} Threads::Threads server_lib ${PQXX_LIB} ${PQ_LIB})

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests.out server/tests/test.cpp)
target_link_libraries(tests.out ${GTEST_LIBRARIES} ${TORCH_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES}
        Threads::Threads server_lib ${PQXX_LIB} ${PQ_LIB} gcov)