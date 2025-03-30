#ifndef _SCENE_H
#define _SCENE_H

#include "GLFW/glfw3.h"
#include "imgui.h"
class Scene {
public:
	Scene(ImGuiIO &ioref, GLFWwindow *windowref);
	~Scene();

	void drawUI();
	bool dirty;

private:
	ImGuiIO &io;
	GLFWwindow *window;
};

#endif
