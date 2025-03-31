#ifndef _CSGNODE_H
#define _CSGNODE_H

#include "types.h"
#include <memory>
#include <vector>

class Scene; // forward decl

class CSGNode: public std::enable_shared_from_this<CSGNode> {
public:
	CSGNode(CSGShape shape);
	CSGNode(CSGOperation op);
	virtual void drawUI(std::shared_ptr<Scene> scene) const = 0;
	virtual ~CSGNode();
	virtual std::optional<CSGOperation> getOperation() const = 0;
	virtual std::optional<CSGShape> getShape() const = 0;
	bool isOperation() const { return std::holds_alternative<CSGOperation>(type); };
	bool isShape() const { return std::holds_alternative<CSGShape>(type); };

protected:
	CSGType type;
};

class CSGShapeNode : public CSGNode {
public:
	CSGShapeNode(CSGShape shape);
	~CSGShapeNode();
	void drawUI(std::shared_ptr<Scene> scene) const;

	std::optional<CSGOperation> getOperation() const { return std::nullopt; };
	std::optional<CSGShape> getShape() const { return std::get<CSGShape>(type); };
};

class CSGOperationNode : public CSGNode {
public:
	CSGOperationNode(CSGOperation op);
	~CSGOperationNode();

	void drawUI(std::shared_ptr<Scene> scene) const;

	std::optional<CSGShape> getShape() const { return std::nullopt; };
	std::optional<CSGOperation> getOperation() const { return std::get<CSGOperation>(type); };
	std::vector<std::shared_ptr<CSGNode>> getChildren() const;

	void clearChildren() { leftchild = {}; rightchild = {}; };
	unsigned int childCount();
	void setLeft(std::shared_ptr<CSGNode> node) { leftchild = node; };
	void setRight(std::shared_ptr<CSGNode> node) { rightchild = node; };

private:
	std::shared_ptr<CSGNode> leftchild;
	std::shared_ptr<CSGNode> rightchild;
};

#endif
