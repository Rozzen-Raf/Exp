project(server-example CXX)

add_executable(${PROJECT_NAME} Exp.cpp)
target_link_libraries(${PROJECT_NAME} AsyncServerApi)