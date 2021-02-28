file(GLOB SRC_FILES *.cpp)

add_library(scenes ${SRC_FILES})

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders/)
target_link_libraries(scenes core)

