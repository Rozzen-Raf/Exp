project(http-parser)

add_library(${PROJECT_NAME} Route.h Route.cpp Request.h Request.cpp HttpRequestParser.h HttpRequestParser.cpp HttpUtils.h HttpUtils.cpp HttpTypes.h)

target_link_libraries(${PROJECT_NAME} core)

set(PARSE_INCLUDE_DIR "./")
target_include_directories(${PROJECT_NAME} PUBLIC ${PARSE_INCLUDE_DIR})
