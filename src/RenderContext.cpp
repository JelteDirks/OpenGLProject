#include "types.h"
#include "util.h"
#include "RenderContext.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

RenderContext::RenderContext()
{

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VertexBuffer vb(vertices, sizeof(vertices));
    IndexBuffer id(elements, sizeof(elements));

    // exits with error if shaders do not compile
    shaderProgram = std::make_unique<ShaderProgram>
        (TOSTRING(SHADER_PATH) "simple.vert",
         TOSTRING(SHADER_PATH) "csg.frag");

    GLuint program = shaderProgram->getProgram();

    const GLint vpos_location = glGetAttribLocation(program, "aPos");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void*) offsetof(Vertex, pos));

    glBindVertexArray(0);
}

RenderContext::~RenderContext()
{
}

void RenderContext::drawUI(Scene &scene, GLFWwindow &window)
{
    if (ImGui::CollapsingHeader("Render Settings")) {
        ImGui::SliderFloat("Smoothing factor", &smoothingFactor, 0.00001f, 0.01f, "%0.8f");
    }
}

void RenderContext::use() const
{
    shaderProgram->use();
    glBindVertexArray(VAO);
}

void RenderContext::linearizeScene(Scene &scene)
{
}

void RenderContext::render(Scene &scene, GLFWwindow &window)
{
    if (scene.dirty) {
        linearizeScene(scene);
    }

    int width, height;
    glfwGetFramebufferSize(&window, &width, &height);
    glViewport(0, 0, width, height);
    shaderProgram->setUniform2f("window_dimensions", width, height);
    shaderProgram->setUniform1f("t_elapsed", glfwGetTime());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
