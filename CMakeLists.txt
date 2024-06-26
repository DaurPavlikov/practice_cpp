cmake_minimum_required(VERSION 3.29 FATAL_ERROR)

set(PROJECT_NAME Practice)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/renderer/shader.cpp
    src/renderer/shader.hpp
    src/renderer/texture_2d.cpp
    src/renderer/texture_2d.hpp
    src/renderer/sprite.cpp
    src/renderer/sprite.hpp
    src/renderer/animated_sprite.cpp
    src/renderer/animated_sprite.hpp
    src/resources/resources_manager.cpp
    src/resources/resources_manager.hpp
    src/resources/stb_image.h 
    )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(lib/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(lib/glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)