project(tests)
if(UNIX)
	file(GLOB TEST_FILES tests/*.cpp tests/*.h)

	add_executable(${PROJECT_NAME} ${TEST_FILES})

	find_package(Catch2)

	target_link_libraries(${PROJECT_NAME} Catch2::Catch2)
	target_link_libraries(${PROJECT_NAME} core)
	target_link_libraries(${PROJECT_NAME} json-parser)

    add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

	include(CTest)
	include(Catch)
endif(UNIX)