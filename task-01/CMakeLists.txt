cmake_minimum_required(VERSION 3.7)
project(hw01_mandelbrot)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

find_package(OpenGL REQUIRED)

#find_package(glm REQUIRED)
#include_directories(${GLM_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp src/MainView.cpp src/MainView.h src/ShadersLoader.cpp src/ShadersLoader.h)
add_executable(hw01_mandelbrot ${SOURCE_FILES})
target_link_libraries(hw01_mandelbrot ${OPENGL_LIBRARY})
