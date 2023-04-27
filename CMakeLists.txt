cmake_minimum_required (VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

set(PROJECT_NAME Exp)
set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external") 
project (${PROJECT_NAME} VERSION 0.0.1)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(TARGET_NAME ${PROJECT_NAME})

file(GLOB SOURCE_FILES
./*.cpp)

file(GLOB HEADER_FILES 
./*.h
)

add_executable (${TARGET_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIR})

target_include_directories(${TARGET_NAME} PRIVATE
"${EXTERNAL_DIR}/json/single_include/nlohmann")