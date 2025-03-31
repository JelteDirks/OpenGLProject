#include "RenderContext.h"
#include "Scene.h"
#include "imgui.h"

Scene::Scene(ImGuiIO &ioref, GLFWwindow &windowref, RenderContext &rendercontext)
:io(ioref), window(windowref), renderContext(rendercontext)
{
}

Scene::~Scene()
{
}

void Scene::render()
{
    renderContext.render(*this, window);
    ImGui::Begin("Configuration");
    drawUI();
    renderContext.drawUI(*this, window);
    ImGui::End();
}

void Scene::drawUI()
{
    ImGui::Text("Framerate %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);

    if (ImGui::CollapsingHeader("Scene settings")) {
        ImGui::ColorEdit3("Background colour", (float*)&backgroundColour);
        ImGui::SliderFloat3("Camera position", (float*)&cameraPosition, -10.0, 10.0);
        ImGui::SliderFloat3("Look at", (float*)&lookAt, -10.0, 10.0);
        ImGui::SliderFloat("Field of view", (float*)&FOV, 40.0, 160.);

        if (ImGui::Button("Test render")) {
            this->dirty = true;
        }
    }
}
