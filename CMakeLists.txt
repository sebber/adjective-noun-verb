cmake_minimum_required(VERSION 3.27)
project(HelloCPP)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CSS_STANDARD_REQUIRED true)

find_package(SDL2 REQUIRED CONFIG)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2-static)

