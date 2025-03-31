#include "CSGNode.h"
#include <memory>
#include <vector>

CSGShapeNode::CSGShapeNode(CSGShape shape)
:CSGNode(shape)
{
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

CSGNode::CSGNode(CSGOperation op)
:type(op)
{
}

const std::vector<std::shared_ptr<CSGNode>> CSGOperationNode::getChildren() const
{
    std::vector<std::shared_ptr<CSGNode>> children{};
    if (leftchild) {
        children.push_back(*leftchild);  // No need to create a new instance
    }
    if (rightchild) {
        children.push_back(*rightchild);
    }
    return children;
}

CSGShapeNode::~CSGShapeNode() {
}

CSGOperationNode::~CSGOperationNode() {
}
