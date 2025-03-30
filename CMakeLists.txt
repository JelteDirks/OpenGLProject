cmake_minimum_required(VERSION 3.10)

project(OpenGLProject LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

add_executable(Gelly 
  src/main.cpp 
  src/VertexBuffer.cpp
  src/IndexBuffer.cpp
  src/util.cpp
  src/ShaderProgram.cpp
  external/glad/src/glad.c)

add_subdirectory(external/glfw)

target_compile_definitions(Gelly PRIVATE GL_SILENCE_DEPRECATION)

target_include_directories(Gelly PRIVATE external/glfw/include)
target_include_directories(Gelly PRIVATE external/glad/include)
target_include_directories(Gelly PRIVATE external/KHR/include)

target_link_libraries(Gelly PRIVATE glfw
  "-framework OpenGL"
  "-framework Cocoa"
  "-framework IOKit"
  "-framework CoreVideo"
)

set(SHADER_PATH "${CMAKE_SOURCE_DIR}/src/shaders/")
add_compile_definitions(SHADER_PATH=${SHADER_PATH})
