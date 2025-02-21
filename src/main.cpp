#include <cstdlib>
#include <glad.h>
#include <GLFW/glfw3.h>
 
#include "IndexBuffer.h"
#include "linmath.h"
#include "VertexBuffer.h"
 
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
 
typedef struct Vertex
{
    vec3 pos;
    vec3 col;
} Vertex;

#define N_VERTICES 4
#define N_ELEMENTS N_VERTICES * 3
 
static const Vertex vertices[N_VERTICES] =
{
    { { -1.0f, -1.0f, 0.0f }, { 1.f, 0.f, 0.f } },
    { {  1.0f, -1.0f, 0.0f }, { 0.f, 1.f, 0.f } },
    { {  1.0f,  1.0f, 0.0f }, { 0.f, 0.f, 1.f } },
    { { -1.0f,  1.0f, 0.0f }, { 1.f, 1.f, 1.f } },
};

static const GLuint elements[N_VERTICES * 3] = { 0, 1, 2, 2, 3, 0 };
 
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int getFileContent(const std::string& filename, std::string& content)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Error: Failed to open file: " << filename << std::endl;
        return 1;
    }

    content.assign((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
    return 0;
}

int main(void)
{
    std::string fragShader;
    if (getFileContent("src/shaders/simple.frag", fragShader)) {
        exit(EXIT_FAILURE);
    }

    std::string vertShader;
    if (getFileContent("src/shaders/simple.vert", vertShader)) {
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit()) exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    gladLoadGL();

    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwSwapInterval(1);
 
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    VertexBuffer vb(vertices, sizeof(vertices));
    IndexBuffer id(elements, sizeof(elements));
 
    const GLuint glVertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertSource = vertShader.c_str();
    glShaderSource(glVertexShader, 1, &vertSource, NULL);
    glCompileShader(glVertexShader);
 
    const GLuint glFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragSource = fragShader.c_str();
    glShaderSource(glFragmentShader, 1, &fragSource, NULL);
    glCompileShader(glFragmentShader);
 
    const GLuint program = glCreateProgram();
    glAttachShader(program, glVertexShader);
    glAttachShader(program, glFragmentShader);
    glLinkProgram(program);
 
    const GLint vpos_location = glGetAttribLocation(program, "vPos");
    const GLint vcol_location = glGetAttribLocation(program, "vCol");
 
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, col));

    const GLint window_dim_location = glGetUniformLocation(program, "window_dimensions");
    const GLint t_elapsed_location = glGetUniformLocation(program, "t_elapsed");

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glUniform2f(window_dim_location, width, height);
        float t_elapsed = glfwGetTime();
        glUniform1f(t_elapsed_location, t_elapsed);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vertex_array);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
