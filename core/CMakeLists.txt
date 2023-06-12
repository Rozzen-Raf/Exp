cmake_minimum_required (VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

set(TARGET_NAME "core")
project (${TARGET_NAME} VERSION 0.0.1)

set(INCLUDE_DIR "./include" "./include/io" "./include/api" "./include/task" "./include/engine")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(LIB_TARGET_NAME ${TARGET_NAME})

file(GLOB SOURCE_FILES
./src/*.cpp
./src/engine/*.cpp
./src/io/*.cpp
./src/task/*.cpp)

file(GLOB HEADER_FILES 
./include/*.h
./include/engine/*.h
./include/io/*.h
./include/task/*.h
./include/api/*.h
)

add_library (${LIB_TARGET_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

set_property(TARGET ${LIB_TARGET_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${LIB_TARGET_NAME} PUBLIC ${INCLUDE_DIR})