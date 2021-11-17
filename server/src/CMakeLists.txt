cmake_minimum_required(VERSION 3.16)

project(server_static_lib)

set(SOURCE_LIB Server.cpp Connection.cpp Handlers.cpp)

add_library(server_lib STATIC ${SOURCE_LIB})
