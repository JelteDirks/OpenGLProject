#include "RenderContext.h"
#include "Scene.h"
#include "imgui.h"

Scene::Scene(ImGuiIO &ioref, GLFWwindow &windowref, RenderContext &rendercontext)
:io(ioref), window(windowref), renderContext(rendercontext)
{ }

Scene::~Scene()
{ }

void Scene::addNode(std::shared_ptr<CSGNode> node)
{ nodes.push_back(node); }

void Scene::render()
{
    renderContext.render(shared_from_this(), window);
    ImGui::Begin("Configuration");
    drawUI();
    renderContext.drawUI(shared_from_this(), window);
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
    }

    for (const auto &node : nodes) {
        if (node->isOperation()) {
            auto operation = std::static_pointer_cast<CSGOperationNode>(node);
            operation->drawUI(shared_from_this());
        } else if (node->isShape()) {
            auto shapenode = std::static_pointer_cast<CSGShapeNode>(node);
            shapenode->drawUI(shared_from_this());
        }
    }


    if (ImGui::Button("Test render")) {
        this->dirty = true;
    }
}
