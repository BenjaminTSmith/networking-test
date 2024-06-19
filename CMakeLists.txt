cmake_minimum_required(VERSION 3.22.1)
project(networking)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_DEBUG_FLAGS "-O0 -Wall")
set(CMAKE_CXX_RELEASE_FLAGS "-O2")

add_executable(server src/server.c)
add_executable(client src/client.c)
