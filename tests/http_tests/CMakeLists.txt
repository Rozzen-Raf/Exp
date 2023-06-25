project(http_tests)
if(UNIX)
	file(GLOB TEST_FILES ./*.cpp ./*.h ../*.cpp ./*.h)

	set(INCLUDE_DIR "./" "../")
	add_executable(${PROJECT_NAME} ${TEST_FILES})

	find_package(Catch2)

	target_link_libraries(${PROJECT_NAME} Catch2::Catch2)
	target_link_libraries(${PROJECT_NAME} core)
	target_link_libraries(${PROJECT_NAME} http-parser)
    target_link_libraries(${PROJECT_NAME} json-parser)
    
	target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})
	
    add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

	include(CTest)
	include(Catch)
endif(UNIX)