#ifndef _RENDERCONTEXT_H
#define _RENDERCONTEXT_H

#include "GLFW/glfw3.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "types.h"
#include <memory>
#include <vector>

typedef struct Vertex
{
    glm::vec3 pos;
} Vertex;

constexpr int N_VERTICES = 4;
constexpr int N_ELEMENTS = N_VERTICES * 3;
constexpr Vertex vertices[N_VERTICES] =
    {
	{ { -1.0f, -1.0f, 0.0f } },
	{ {  1.0f, -1.0f, 0.0f } },
	{ {  1.0f,  1.0f, 0.0f } },
	{ { -1.0f,  1.0f, 0.0f } },
    };
constexpr GLuint elements[N_VERTICES * 3] = { 0, 1, 2, 2, 3, 0 };

class RenderContext {
public:
    RenderContext();
    virtual ~RenderContext();
    void render(Scene &scene, GLFWwindow &window);
    void use() const;
    void drawUI(Scene &scene, GLFWwindow &window);

    static std::shared_ptr<LinearCSGTreeNode> makeCSGTreeNode(const std::shared_ptr<CSGNode> &node);

private:
    void linearizeCSGNode(const std::shared_ptr<CSGNode> &node);
    void linearizeScene(Scene &scene);

    GLuint VAO;
    std::unique_ptr<ShaderProgram> shaderProgram;
    float smoothingFactor = 0.001f;
    std::vector<std::shared_ptr<LinearCSGTreeNode>> linearScene{};
};

#endif
