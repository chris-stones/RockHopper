
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Translate : public Object {

	glm::vec3 translation;

public:

	virtual void AcceptVisitor(IVisitor * visitor) { visitor->Visit(this); }

	Translate() {}
	Translate(const glm::vec3 &translation) : translation(translation) {}

	const glm::vec3& Get() const { return translation; }
	void Set(const glm::vec3 &translation) { this->translation = translation; }
};

}}}
