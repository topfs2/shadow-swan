cmake_minimum_required(VERSION 2.6)
project(ShadowSwan)

add_subdirectory(deps)

set(LIBRARIES
  glfw
  assimp
  stb
  glm
  glad
  dl
)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(main ${SOURCES} main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main ${LIBRARIES})
