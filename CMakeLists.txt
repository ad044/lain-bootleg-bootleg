cmake_minimum_required(VERSION 3.2)

project(lain-bootleg-bootleg)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

set(CMAKE_CXX_STANDARD 18)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_NAME ${PROJECT_NAME})

find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCES
   src/main.c
   src/engine.c
   src/hashmap.c
   src/input.c
   src/menu.c
   src/resource_cache.c
   src/scene.c
   src/shader.c
   src/sprite.c
   src/stb_image.c
   src/text.c
   src/texture.c
   src/window.c
)

set(HEADERS
   src/engine.h
   src/hashmap.h
   src/input.h
   src/menu.h
   src/resource_cache.h
   src/scene.h
   src/shader.h
   src/sprite.h
   src/stb_image.h
   src/text.h
   src/texture.h
   src/vector2d.h
   src/window.h
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(${PROJECT_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${LIBRARY_NAME} ${OPENGL_gl_LIBRARY} ${GLFW_glfw_LIBRARY} ${GLEW_LIBRARIES} cglm m)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/cglm/)



