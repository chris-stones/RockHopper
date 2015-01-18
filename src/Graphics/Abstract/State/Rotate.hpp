
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Rotate : public Object {

	glm::mat4 rotation;

public:

	virtual void AcceptVisitor(IVisitor * visitor) { visitor->Visit(this); }

	Rotate() {}
	Rotate(const glm::mat4 &rotation) : rotation(rotation) {}

	const glm::mat4& Get() const { return rotation; }
	void Set(const glm::mat4 &rotation) { this->rotation = rotation; }
};

}}}
