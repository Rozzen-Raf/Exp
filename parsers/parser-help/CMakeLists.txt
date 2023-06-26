project(parser-help CXX)

add_library(${PROJECT_NAME} ParserUtils.h)

set(PARSE_INCLUDE_DIR "./")

target_include_directories(${PROJECT_NAME} PUBLIC ${PARSE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} core)
target_link_libraries(${PROJECT_NAME} json-parser)