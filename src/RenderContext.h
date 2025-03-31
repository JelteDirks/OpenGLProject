#ifndef _RENDERCONTEXT_H
#define _RENDERCONTEXT_H

#include "GLFW/glfw3.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "types.h"
#include <memory>
#include <vector>

class RenderContext {
public:
	RenderContext();
	virtual ~RenderContext();
	void render(Scene &scene, GLFWwindow &window);
	void use() const;
	void drawUI(Scene &scene, GLFWwindow &window);
	void linearizeScene(Scene &scene);

private:
	GLuint VAO;
	std::unique_ptr<ShaderProgram> shaderProgram;
	float smoothingFactor = 0.001f;
	std::vector<LinearCSGTreeNode> linearScene{};
};

#endif
