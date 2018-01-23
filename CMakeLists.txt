cmake_minimum_required(VERSION 2.6)
project(Scene3D)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

find_package(OpenGL REQUIRED)

include_directories(
        .
)

find_package(glm REQUIRED)
include_directories(include ${GLM_INCLUDE_DIRS})

add_subdirectory(external/glfw-3.1.2)

include_directories(
        external/glfw-3.1.2/include/GLFW/
        external/glew-1.13.0/include/
)

### GLEW ###

set(GLEW_SOURCE external/glew-1.13.0/src/glew.c)

set(GLEW_HEADERS)

add_library(GLEW STATIC
        ${GLEW_SOURCE}
        ${GLEW_INCLUDE}
        )

target_link_libraries(GLEW ${OPENGL_LIBRARY} ${EXTRA_LIBS})


add_executable(Scene3D
        src/main.cpp)

target_link_libraries(Scene3D ${OPENGL_LIBRARY} glfw GLEW)
