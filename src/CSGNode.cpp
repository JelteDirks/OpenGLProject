#include "CSGNode.h"
#include "imgui.h"
#include <memory>
#include <vector>

CSGShapeNode::CSGShapeNode(CSGShape shape)
:CSGNode(shape)
{
}

void CSGOperationNode::drawUI(std::shared_ptr<Scene> scene) const
{
    CSGOperation op = std::get<CSGOperation>(type);
    if (ImGui::TreeNode(to_string(op))) {
        ImGui::Text("Operation settings here");
        for (auto childNode : getChildren()) {
            childNode->drawUI(scene);
        }
        ImGui::TreePop();
    }
}

void CSGShapeNode::drawUI(std::shared_ptr<Scene> scene) const
{
    CSGShape shape = std::get<CSGShape>(type);
    if (ImGui::TreeNode(to_string(shape))) {
        ImGui::Text("Shape settings here");
        ImGui::TreePop();
    }
}

CSGOperationNode::CSGOperationNode(CSGOperation op)
:CSGNode(op)
{
}

CSGNode::~CSGNode()
{
}

CSGNode::CSGNode(CSGShape shape)
:type(shape)
{
}

unsigned int CSGOperationNode::childCount()
{
    unsigned int count = 0;
    if (leftchild) {
        ++count;
    }
    if (rightchild) {
        ++count;
    }
    return count;
}

CSGNode::CSGNode(CSGOperation op)
:type(op)
{
}

std::vector<std::shared_ptr<CSGNode>> CSGOperationNode::getChildren() const
{
    std::vector<std::shared_ptr<CSGNode>> children;
    if (leftchild) {
        children.push_back(leftchild);
    }
    if (rightchild) {
        children.push_back(rightchild);
    }
    return children;
}

CSGShapeNode::~CSGShapeNode() {
}

CSGOperationNode::~CSGOperationNode() {
}
