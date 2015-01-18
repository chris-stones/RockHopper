
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Sprite : public Object {

	std::shared_ptr<IResource> resource;

public:
	virtual void AcceptVisitor(IVisitor * visitor) { visitor->Visit(this); }

	Sprite(std::shared_ptr<IResource> resource);

	virtual ~Sprite();

	virtual glm::vec3 AlignmentCorrection() const;

	class Impl;

	Impl * GetImpl() { return impl; }

private:

	Impl * impl;
};

}}}
