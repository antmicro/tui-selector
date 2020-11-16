cmake_minimum_required(VERSION 2.8)
project(tui-selector)

add_subdirectory(third-party/rapidjson)

find_package(RapidJSON)
find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR} ${RapidJSON_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
    src/tui-selector.cpp
)
add_dependencies(${PROJECT_NAME} rapidjson)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES} rapidjson)
