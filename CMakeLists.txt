cmake_minimum_required(VERSION 2.6)
project(Project)
add_definitions(-std=c++14)
add_definitions(-Os)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

include(FindPkgConfig)

pkg_search_module(GLFW3 REQUIRED glfw3)
include_directories(${GLFW3_INCLUDE_DIRS})

pkg_search_module(GLEW REQUIRED glew)
include_directories(${GLEW_INCLUDE_DIRS})

pkg_search_module(GLU REQUIRED glu)
include_directories(${GLU_INCLUDE_DIRS})

pkg_search_module(GL REQUIRED gl)
include_directories(${GL_INCLUDE_DIRS})

FIND_PACKAGE(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
add_definitions(-DGLM_FORCE_RADIANS -DGLM_ENABLE_EXPERIMENTAL)

pkg_search_module(ASSIMP REQUIRED assimp)
include_directories(${ASSIMP_INCLUDE_DIRS})

FIND_PACKAGE(Threads REQUIRED)

set(LIBRARIES ${GLFW3_LIBRARIES} ${GLEW_LIBRARIES} ${GLU_LIBRARIES} ${GL_LIBRARIES} ${ASSIMP_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

set(SOURCES src/Cubemap.cpp src/FrameBuffer.cpp src/ModelLoader.cpp src/Geometry.cpp src/GeometryData.cpp src/GeometryFactory.cpp src/Image.cpp src/Shader.cpp src/Material.cpp src/Mesh.cpp src/TrackballCamera.cpp src/scenes/SceneFactory.cpp src/scenes/BaseScene.cpp src/scenes/SampleScene.cpp src/scenes/PBRScene.cpp)

add_executable(main ${SOURCES} main.cpp)
include_directories(includes)
target_link_libraries(main ${LIBRARIES})
