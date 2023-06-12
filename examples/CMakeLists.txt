project(server-example CXX)


add_executable(${PROJECT_NAME} Exp.cpp ApiCommandExample.cpp ApiCommandExample.h)
target_link_libraries(${PROJECT_NAME} core)
target_link_libraries(${PROJECT_NAME} json-parser)

set(LIB_Name examples_lib)
add_library(${LIB_Name} ApiCommandExample.cpp ApiCommandExample.h)
target_link_libraries(${LIB_Name} core)
target_link_libraries(${LIB_Name} json-parser)
target_include_directories(${LIB_Name} PUBLIC "./")
