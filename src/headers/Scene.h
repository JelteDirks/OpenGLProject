#ifndef _SCENE_H
#define _SCENE_H

#include "CSGNode.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include <glm/vec3.hpp>
#include <vector>

class RenderContext; // forward decl

class Scene: public std::enable_shared_from_this<Scene> {
public:
	Scene(ImGuiIO &ioref, GLFWwindow &windowref, RenderContext &rendercontext);
	~Scene();

	void drawUI();
	bool dirty;
	void render();

	void addNode(std::shared_ptr<CSGNode> node);
	const std::vector<std::shared_ptr<CSGNode>> &getNodes() { return nodes; };

private:
	ImGuiIO &io;
	GLFWwindow &window;
	RenderContext &renderContext;

	std::vector<std::shared_ptr<CSGNode>> nodes{};

	glm::vec3 backgroundColour = glm::vec3(0.3f);
	glm::vec3 cameraPosition = glm::vec3(0.0f);
	glm::vec3 lookAt = glm::vec3(0.0f);
	float FOV = 60;
};

#endif
