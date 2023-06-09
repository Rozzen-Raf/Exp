project(json-parser)

add_library(${PROJECT_NAME} JsonParser.cpp JsonParser.h)

target_include_directories(${PROJECT_NAME} PUBLIC "./")
target_include_directories(${PROJECT_NAME} PUBLIC
"${EXTERNAL_DIR}/json/single_include/nlohmann")

target_link_libraries(${PROJECT_NAME} core)