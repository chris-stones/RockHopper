
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Projection : public Object {

	glm::mat4 projection;

public:

	virtual void AcceptVisitor(IVisitor * visitor) { visitor->Visit(this); }

	Projection() {}
	Projection(const glm::mat4 &projection) : projection(projection) {}

	const glm::mat4& Get() const { return projection; }
	void Set(const glm::mat4 &projection) { this->projection = projection; }
};

}}}
