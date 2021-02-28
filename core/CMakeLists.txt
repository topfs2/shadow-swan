file(GLOB SRC_FILES *.cpp)

add_library(core ${SRC_FILES})
target_link_libraries(core PUBLIC glad glm gli dl imgui glfw stb assimp)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
