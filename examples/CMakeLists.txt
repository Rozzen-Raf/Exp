project(server-example CXX)

add_executable(${PROJECT_NAME} Exp.cpp ApiCommandExample.cpp ApiCommandExample.h)
target_link_libraries(${PROJECT_NAME} core)
target_link_libraries(${PROJECT_NAME} json-parser)

