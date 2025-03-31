cmake_minimum_required(VERSION 3.10)

project(OpenGLProject LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Gelly
  src/main.cpp 
  src/VertexBuffer.cpp
  src/IndexBuffer.cpp
  src/util.cpp
  src/ShaderProgram.cpp
  src/Scene.cpp
  src/CSGNode.cpp
  src/RenderContext.cpp
  external/glad/src/glad.c
  external/imgui/imgui.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_opengl3.cpp
)

add_subdirectory(external/glfw)

target_compile_definitions(Gelly PRIVATE GL_SILENCE_DEPRECATION)

target_include_directories(Gelly PRIVATE
  src/headers
  external/glfw/include
  external/glad/include
  external/KHR/include
  external/imgui
  external/imgui/backends
  external/glm/glm
)

target_link_libraries(Gelly PRIVATE glfw
  "-framework OpenGL"
  "-framework Cocoa"
  "-framework IOKit"
  "-framework CoreVideo"
)

set(SHADER_PATH "${CMAKE_SOURCE_DIR}/src/shaders/")
add_compile_definitions(SHADER_PATH=${SHADER_PATH})
