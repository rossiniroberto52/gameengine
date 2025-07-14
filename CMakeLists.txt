cmake_minimum_required(VERSION 3.15)
project(Engine2D)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Engine2D
  src/engine/core/Game.cpp
  src/engine/core/GameObject.cpp
  src/engine/core/Render.cpp
  src/engine/core/Window.h
  src/engine/core/Window.cpp
  src/engine/core/Shader.h
  src/engine/core/Shader.cpp
  src/engine/gui/ImageSelector.cpp
)

target_include_directories(Engine2D PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core
  ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/gui
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
)

find_package(SDL2_image REQUIRED)

target_include_directories(Engine2D PUBLIC
  ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(Engine2D
  glad
  glfw
  GLEW
  GL
  SDL2
  SDL2_image
  ${SDL2_IMAGE_LIBRARIES}
)

add_library(glad STATIC
  third_party/glad/src/glad.c
)

target_include_directories(glad PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glad/include
)
