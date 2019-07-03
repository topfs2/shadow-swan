cmake_minimum_required(VERSION 2.6)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)

add_library(gli INTERFACE)
target_include_directories(gli INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/gli)

add_library(glad ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glfw)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/assimp)

add_library(imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
