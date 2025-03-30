#ifndef _SCENE_H
#define _SCENE_H

#include "GLFW/glfw3.h"
#include "imgui.h"

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
};

#endif
