project(json-parser)

add_library(${PROJECT_NAME} JsonParser.cpp JsonParser.h)

set(PARSE_INCLUDE_DIR "./" "${EXTERNAL_DIR}/json/single_include/nlohmann")

target_include_directories(${PROJECT_NAME} PUBLIC ${PARSE_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} core)
target_link_libraries(${PROJECT_NAME} parser-help)