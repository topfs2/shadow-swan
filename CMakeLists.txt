cmake_minimum_required(VERSION 2.6)
project(ShadowSwan)

add_subdirectory(deps)

set(LIBRARIES
  glfw
  assimp
  stb
  glm
  gli
  glad
  dl
  imgui
)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(main ${SOURCES} main.cpp)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(main PRIVATE cxx_std_17)
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(main ${LIBRARIES})
