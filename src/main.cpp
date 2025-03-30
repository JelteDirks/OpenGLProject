#include <cstdlib>

#include <glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
 
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "types.h"
#include "util.h"
 
#include <iostream>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>



int main(void)
{
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

    // exits with error if shaders do not compile
    std::unique_ptr<ShaderProgram> shaderProgram = std::make_unique<ShaderProgram>
        (TOSTRING(SHADER_PATH) "simple.vert",
         TOSTRING(SHADER_PATH) "csg.frag");

    GLuint program = shaderProgram->getProgram();
    shaderProgram->use();

    const GLint vpos_location = glGetAttribLocation(program, "aPos");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram->use();
        shaderProgram->setUniform2f("window_dimensions", width, height);
        shaderProgram->setUniform1f("t_elapsed", glfwGetTime());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
