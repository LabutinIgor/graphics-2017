cmake_minimum_required(VERSION 2.6)
project(GodRays)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

find_package(OpenGL REQUIRED)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(OPENGL_LIBRARY
            ${OPENGL_LIBRARY}
            -lGL -lGLU -lXrandr -lXext -lX11 -lrt
            ${CMAKE_DL_LIBS}
            ${GLFW_LIBRARIES}
            )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OPENGL_LIBRARY
            ${OPENGL_LIBRARY}
            ${CMAKE_DL_LIBS}
            ${GLFW_LIBRARIES}
            )
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

find_package(glm REQUIRED)
include_directories(include ${GLM_INCLUDE_DIRS})

add_subdirectory(external/glfw-3.1.2)

include_directories(
        external/
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


add_executable(GodRays
        src/main.cpp
        src/controls.cpp
        src/controls.hpp
        src/MainView.cpp
        src/MainView.h
        src/load_shaders.cpp
        src/load_shaders.h
        src/Object3D.cpp
        src/Object3D.h
        src/PointLight.cpp
        src/PointLight.h
        external/FrameBuffer.cpp
        external/FrameBuffer.h
        )

target_link_libraries(GodRays ${OPENGL_LIBRARY} glfw GLEW)
