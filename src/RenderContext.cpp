#include "types.h"
#include "util.h"
#include "RenderContext.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <iostream>

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

void RenderContext::drawUI(std::shared_ptr<Scene> scene, GLFWwindow &window)
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

std::shared_ptr<LinearCSGTreeNode> RenderContext::makeCSGTreeNode(const std::shared_ptr<CSGNode> &node)
{
    auto lnode = std::make_shared<LinearCSGTreeNode>();

    if (auto op = node->getOperation()) {
        lnode->op = static_cast<int>(*op);
        lnode->shape = static_cast<int>(CSGShape::NOSH);
    } else if (auto shape = node->getShape()) {
        lnode->shape = static_cast<int>(*shape);
        lnode->op = static_cast<int>(CSGOperation::NOOP);
    } else {
        throw std::logic_error("can not make linear csgtree node from this");
    }

    return lnode;
}

void RenderContext::linearizeCSGNode(const std::shared_ptr<CSGNode> &node)
{
    if (auto op = node->getOperation()) {
        for (auto child : static_cast<CSGOperationNode>(*op).getChildren()) {
            linearizeCSGNode(child);
        }
        auto lnode = RenderContext::makeCSGTreeNode(node);
        linearScene.push_back(lnode);
    } else if (auto shape = node->getShape()) {
        auto lnode = RenderContext::makeCSGTreeNode(node);
        linearScene.push_back(lnode);
    } else {
        throw std::logic_error("should be shape or operation");
    }
}

void RenderContext::linearizeScene(std::shared_ptr<Scene> scene)
{
    std::cout << "linearizing...\n";
    linearScene.clear();
    int counter = 0;
    for (const auto &node : scene->getNodes()) {
        linearizeCSGNode(node);
        ++counter;
    }

    for (int i = 0; i < counter - 1; ++i) {
        auto unionShape = std::make_shared<CSGOperationNode>(CSGOperation::UNI);
        linearScene.push_back(RenderContext::makeCSGTreeNode(unionShape));
    }

    for (auto x : linearScene) {
        std::cout << x->op << x->shape << std::endl;
    }
}

void RenderContext::render(std::shared_ptr<Scene> scene, GLFWwindow &window)
{
    if (scene->dirty) {
        linearizeScene(scene);
        scene->dirty = false;
    }

    int width, height;
    glfwGetFramebufferSize(&window, &width, &height);
    glViewport(0, 0, width, height);
    shaderProgram->setUniform2f("window_dimensions", width, height);
    shaderProgram->setUniform1f("t_elapsed", glfwGetTime());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
