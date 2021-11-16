cmake_minimum_required(VERSION 3.18)

project(db_static_lib)

set(SOURCE_LIB db_implementation.cpp)

add_library(db_lib STATIC ${SOURCE_LIB})
