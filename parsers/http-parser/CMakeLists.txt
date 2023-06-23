project(http-parser)

file(GLOB SOURCE_FILES
./src/*.cpp
./include/*.h
)
add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} core)

set(PARSE_INCLUDE_DIR "./include")
target_include_directories(${PROJECT_NAME} PUBLIC ${PARSE_INCLUDE_DIR})
