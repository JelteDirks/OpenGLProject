#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include <cstdlib>

#include <glad.h>
#include <GLFW/glfw3.h>
 
#include "RenderContext.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "util.h"
#include "Scene.h"
 
#include <iostream>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1200, 800, "CSG Visualizer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    gladLoadGL();
    std::cout << glGetString(GL_VERSION) << std::endl;
    glfwSwapInterval(1);

    auto renderContext = std::make_unique<RenderContext>();
    renderContext->use();

    std::shared_ptr<Scene> scene = std::make_shared<Scene>(io, *window, *renderContext);
    auto box = std::make_shared<CSGShapeNode>(CSGShape::BOX);
    auto sphere = std::make_shared<CSGShapeNode>(CSGShape::SPHERE);
    auto uniNode = std::make_shared<CSGOperationNode>(CSGOperation::UNI);

    auto box2 = std::make_shared<CSGShapeNode>(CSGShape::BOX);
    auto sphere2 = std::make_shared<CSGShapeNode>(CSGShape::SPHERE);

    uniNode->setLeft(sphere2);
    uniNode->setRight(sphere);

    scene->addNode(uniNode);
    scene->addNode(box2);
    scene->addNode(box);

    auto x = uniNode->getChildren();
    std::cout << "x.size(): " << x.size() << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        scene->render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
