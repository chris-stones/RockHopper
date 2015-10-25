
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Update : public Object {

	IUpdatedNode * updatedNode {nullptr};

public:
	virtual void AcceptVisitor(IVisitor * visitor) { visitor->Visit(this); }

	Update() {}
	Update(IUpdatedNode *updatedNode)
		:	updatedNode(updatedNode)
	{}

	IUpdatedNode * Get() { return updatedNode; }
	void Set(IUpdatedNode * updatedNode) { this->updatedNode = updatedNode; }
};

}}}
