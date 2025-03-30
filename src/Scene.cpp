#include <glm/glm.hpp>
#include "Scene.h"
#include "imgui.h"

Scene::Scene(ImGuiIO &ioref, GLFWwindow *windowref)
:io(ioref), window(windowref)
{
}

Scene::~Scene()
{
}

void Scene::drawUI()
{
    static float f = 0.0f;
    static int counter = 0;
    static glm::vec3 clear_color;

    ImGui::Begin("Hello, world!");

    ImGui::Text("This is some useful text.");

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);

    if (ImGui::Button("Button")) {
        counter++;
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
    }
