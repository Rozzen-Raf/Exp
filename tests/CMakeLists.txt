project(tests)
if(UNIX)
	file(GLOB TEST_FILES tests/*.cpp tests/*.h tests/http_tests/*.cpp tests/http_tests/*.h)

	set(INCLUDE_DIR "./tests/" "./tests/http_tests")

	add_executable(${PROJECT_NAME} ${TEST_FILES})

	find_package(Catch2)

	target_link_libraries(${PROJECT_NAME} Catch2::Catch2)
	target_link_libraries(${PROJECT_NAME} core)
	target_link_libraries(${PROJECT_NAME} json-parser)
	target_link_libraries(${PROJECT_NAME} http-parser)
	target_link_libraries(${PROJECT_NAME} examples_lib)

	target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
	
    add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

	include(CTest)
	include(Catch)
endif(UNIX)