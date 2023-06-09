cmake_minimum_required (VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

project(serverApi)

set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external") 

add_subdirectory(core "${CMAKE_BINARY_DIR}/core")
add_subdirectory(examples "${CMAKE_BINARY_DIR}/examples")
add_subdirectory(parsers "${CMAKE_BINARY_DIR}/parsers")