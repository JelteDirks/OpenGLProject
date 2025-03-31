#ifndef _SCENE_H
#define _SCENE_H

#include "GLFW/glfw3.h"
#include "imgui.h"
#include <glm/vec3.hpp>

class RenderContext; // forward decl

class Scene {
public:
	Scene(ImGuiIO &ioref, GLFWwindow &windowref, RenderContext &rendercontext);
	~Scene();

	void drawUI();
	bool dirty;
	void render();

private:
	ImGuiIO &io;
	GLFWwindow &window;
	RenderContext &renderContext;

	glm::vec3 backgroundColour = glm::vec3(0.3f);
	glm::vec3 cameraPosition = glm::vec3(0.0f);
	glm::vec3 lookAt = glm::vec3(0.0f);
	float FOV = 60;
};

#endif
