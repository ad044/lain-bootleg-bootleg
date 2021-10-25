cmake_minimum_required(VERSION 3.2)

project(lain-bootleg-bootleg)

set(CMAKE_CXX_STANDARD 18)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if (MINGW)
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif (MINGW)

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
   src/quad.c
   src/state.c
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
   src/window.h
   src/quad.h
   src/util.h
   src/state.h
)

add_executable(${PROJECT_NAME} ${SOURCES})

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
  target_compile_options(${PROEJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/cglm/ EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE cglm)

add_definitions(-DGLEW_STATIC)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glew-cmake EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
