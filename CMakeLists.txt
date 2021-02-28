cmake_minimum_required(VERSION 3.10.2)
project(ShadowSwan)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(deps)
add_subdirectory(core)
add_subdirectory(scenes)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/)

add_executable(${PROJECT_NAME} main.cpp)
#target_include_directories(${PROJECT_NAME} PRIVATE "core")
target_link_libraries(${PROJECT_NAME} PUBLIC core scenes glfw glad)
