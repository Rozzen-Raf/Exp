project(json-parser)

add_library(${PROJECT_NAME} JsonParser.cpp JsonParser.h)
set(INCLUDE_DIR "./")
target_link_libraries(${PROJECT_NAME} core)
target_link_libraries(${PROJECT_NAME} parser-help)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})