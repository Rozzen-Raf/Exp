cmake_minimum_required (VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

set(TARGET_NAME "core")
project (${TARGET_NAME} VERSION 0.0.1)

set(INCLUDE_DIR "./include")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(LIB_TARGET_NAME ${TARGET_NAME})

file(GLOB SOURCE_FILES
./src/*.cpp)

file(GLOB HEADER_FILES 
./include/*.hs
)

add_library (${LIB_TARGET_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

set_property(TARGET ${LIB_TARGET_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${LIB_TARGET_NAME} PUBLIC ${INCLUDE_DIR})

if(UNIX)
	set(TEST_TARGET tests)

	file(GLOB TEST_FILES tests/*.cpp tests/*.h)

	add_executable(${TEST_TARGET} ${TEST_FILES})

	find_package(Catch2)

	target_link_libraries(${TEST_TARGET} Catch2::Catch2)
	target_link_libraries(${TEST_TARGET} ${LIB_TARGET_NAME})
	target_link_libraries(${PROJECT_NAME} json-parser)

	target_include_directories(${TEST_TARGET} PUBLIC ${INCLUDE_DIR})

    add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})

	include(CTest)
	include(Catch)
endif(UNIX)