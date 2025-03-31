#ifndef _CSGNODE_H
#define _CSGNODE_H

#include "types.h"
#include <memory>
#include <vector>

class CSGNode: public std::enable_shared_from_this<CSGNode> {
public:
	CSGNode(CSGShape shape);
	CSGNode(CSGOperation op);
	virtual ~CSGNode() = 0;
	virtual std::optional<CSGOperation> getOperation() const = 0;
	virtual std::optional<CSGShape> getShape() const = 0;
	bool isOperation() const { return std::holds_alternative<CSGOperation>(type); };
protected:
	CSGType type;
};

class CSGShapeNode : public CSGNode {
public:
	CSGShapeNode(CSGShape shape);
	~CSGShapeNode();

	std::optional<CSGOperation> getOperation() const { return std::nullopt; };
	std::optional<CSGShape> getShape() const { return std::get<CSGShape>(type); };
};

class CSGOperationNode : public CSGNode {
public:
	CSGOperationNode(CSGOperation op);
	~CSGOperationNode();

	std::optional<CSGShape> getShape() const { return std::nullopt; };
	std::optional<CSGOperation> getOperation() const { return std::get<CSGOperation>(type); };
	const std::vector<std::shared_ptr<CSGNode>> getChildren() const;
private:
	std::optional<std::shared_ptr<CSGNode>> leftchild;
	std::optional<std::shared_ptr<CSGNode>> rightchild;
};

#endif
