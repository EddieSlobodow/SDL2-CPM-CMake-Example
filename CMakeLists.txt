cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# policies
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# create project
project(MyProject)

# set where the compiled files goes
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
LINK_DIRECTORIES(${CMAKE_BINARY_DIR})

# add dependencies
include(external/compilerchecks.cmake)
include(external/cpm.cmake)

# add libraries
include(external/glew.cmake)
include(external/sdl.cmake)
include(external/imgui.cmake)

# add executable
add_executable(demo main.cpp)
add_executable(demo2 main2.cpp)

find_package(OpenGL REQUIRED)
if(EMSCRIPTEN)
    target_link_libraries(demo  PUBLIC SDL2        SDL2_image IMGUI)
    target_link_libraries(demo2 PUBLIC SDL2        ${OPENGL_gl_LIBRARY} ${CMAKE_DL_LIBS} SDL2_image IMGUI)
ELSE()
    target_link_libraries(demo  PUBLIC SDL2-static SDL2_image IMGUI)
    target_link_libraries(demo2 PUBLIC SDL2-static ${OPENGL_gl_LIBRARY} ${CMAKE_DL_LIBS} SDL2_image IMGUI glew)
ENDIF()

file(GLOB IMGS ${PROJECT_SOURCE_DIR}/*.png)
file(COPY ${IMGS} DESTINATION ${CMAKE_BINARY_DIR})