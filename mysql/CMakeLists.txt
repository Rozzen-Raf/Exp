project(mysql CXX)

file(GLOB SOURCE_FILES
"./src/*.cpp"
"./include/*.hpp"
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} core)

set(INCLUDE_DIR "./include")

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

find_package(MySQL REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC  ${MYSQL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${MYSQL_LIBRARIES})