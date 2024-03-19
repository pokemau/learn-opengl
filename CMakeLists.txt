cmake_minimum_required(VERSION 3.27)

project(SlommGL)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        vendor/glad/include
        vendor/glfw-3.4/include
        vendor/stb_image
        vendor/glm
)

add_executable(${PROJECT_NAME}
        src/Main.cpp
        src/Core/GL.cpp
        src/Core/GL.h
        src/Core/Shader.cpp
        src/Core/Shader.h
        vendor/stb_image/Stb.cpp
        src/Core/Texture.cpp
        src/Core/Texture.h
        src/Core/Camera.h
        src/Core/Camera.cpp
        src/Core/Terrain.cpp
        src/Core/Terrain.h
)

############## GLFW ###################
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw-3.4)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw opengl32 gdi32)
endif (WIN32)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE glfw GL)
endif (UNIX)

############## GLAD ###################
add_library(glad STATIC vendor/glad/src/glad.c)
target_include_directories(glad PRIVATE vendor/glad/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)
