project(http-parser)

add_library(${PROJECT_NAME} Route.h Route.cpp)

target_link_libraries(${PROJECT_NAME} core)