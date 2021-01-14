cmake_minimum_required(VERSION 2.8)
project(tui-selector)

add_subdirectory(third-party/rapidjson)

find_package(RapidJSON)
find_package(Curses REQUIRED)
find_library(MENU_LIBRARY menu)

include_directories(
    ${CURSES_INCLUDE_DIR}
    ${RapidJSON_INCLUDE_DIR}
    src/
)

add_executable(${PROJECT_NAME}
    src/tui-menu.cpp
    src/selector-parser.cpp
    src/tui-selector.cpp
)
add_dependencies(${PROJECT_NAME} rapidjson)
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARY} ${MENU_LIBRARY} rapidjson)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
