cmake_minimum_required(VERSION 3.10)

project(OpenGLProject LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)

add_executable(Gelly src/main.cpp external/glad/src/glad.c)

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
