cmake_minimum_required(VERSION 3.2)

project(lain-bootleg-bootleg)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
   src/main.c
   src/engine.c
   src/input.c
   src/menu.c
   src/scene.c
   src/shader.c
   src/sprite.c
   src/stb_image.c
   src/text.c
   src/texture.c
   src/window.c
   src/state.c
   src/kumashoot.c
   src/minigame.c
   src/vector2d.c
   src/random.c
   src/animations.c
   src/dressup.c
   src/theater.c
   src/cJSON.c
   src/resources.c
   src/sound.c
   src/movie.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

option(SYSTEM_GLEW "Use system-installed GLEW instead of vendored one" OFF)

if(SYSTEM_GLEW)
   find_package(PkgConfig REQUIRED)
   pkg_check_modules(GLEW REQUIRED glew)
   target_link_libraries(${PROJECT_NAME} PRIVATE ${GLEW_LIBRARIES})
else()
   add_definitions(-DGLEW_STATIC)
   add_subdirectory(${CMAKE_SOURCE_DIR}/external/glew-cmake EXCLUDE_FROM_ALL)
   target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)
endif()

option(SYSTEM_GLFW "Use system-installed GLFW instead of vendored one" OFF)

if(SYSTEM_GLFW)
   find_package(glfw3 REQUIRED)
else()
   set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
   set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
   set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
   add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw EXCLUDE_FROM_ALL)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

if (UNIX)
   find_package(PkgConfig REQUIRED)
   pkg_check_modules(MPV REQUIRED mpv)
   target_link_libraries(${PROJECT_NAME} PRIVATE ${MPV_LIBRARIES})
endif (UNIX)

if (MINGW)
   set(MPV_DIR ${PROJECT_SOURCE_DIR}/external/mpv/)

   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,-Bdynamic,--no-whole-archive")
   target_include_directories(${PROJECT_NAME} PRIVATE ${MPV_DIR}/include)
   target_link_libraries(${PROJECT_NAME} PRIVATE ${MPV_DIR}/mpv-2.dll)
   add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${MPV_DIR}/mpv-2.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif (MINGW)

