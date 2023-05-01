cmake_minimum_required (VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

set(PROJECT_NAME AsyncServerApi)
set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external") 
project (${PROJECT_NAME} VERSION 0.0.1)

set(INCLUDE_DIR "./include")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(TARGET_NAME ${PROJECT_NAME})
set(LIB_TARGET_NAME "AsyncServerApiLib")

file(GLOB SOURCE_FILES
./src/*.cpp)

file(GLOB HEADER_FILES 
./include/*.h
)

add_executable (${TARGET_NAME} ${SOURCE_FILES} ${HEADER_FILES})
add_library (${LIB_TARGET_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIR})
target_include_directories(${LIB_TARGET_NAME} PRIVATE ${INCLUDE_DIR})

target_include_directories(${TARGET_NAME} PRIVATE
"${EXTERNAL_DIR}/json/single_include/nlohmann")
target_include_directories(${LIB_TARGET_NAME} PRIVATE
"${EXTERNAL_DIR}/json/single_include/nlohmann")

if(UNIX)
	set(TEST_TARGET tests)

	file(GLOB TEST_FILES tests/*.cpp tests/*.h)

	add_executable(${TEST_TARGET} ${TEST_FILES})

	find_package(Catch2)

	target_link_libraries(${TEST_TARGET} Catch2::Catch2)
	target_link_libraries(${TEST_TARGET} ${LIB_TARGET_NAME})

	target_include_directories(${TEST_TARGET} PRIVATE ${INCLUDE_DIR})
	target_include_directories(${TEST_TARGET} PRIVATE
"${EXTERNAL_DIR}/json/single_include/nlohmann"
)

    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})

	include(CTest)
	include(Catch)
endif(UNIX)