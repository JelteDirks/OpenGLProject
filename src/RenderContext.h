#ifndef _RENDERCONTEXT_H
#define _RENDERCONTEXT_H

#include "GLFW/glfw3.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include <memory>

class RenderContext {
public:
	RenderContext();
	virtual ~RenderContext();
	void render(Scene &scene, GLFWwindow &window);
	void use() const;
	void drawUI(Scene &scene, GLFWwindow &window);

private:
	GLuint VAO;
	std::unique_ptr<ShaderProgram> shaderProgram;
};

#endif
