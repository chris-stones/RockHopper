
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Scale : public Object {

	float scale;
public:

	virtual void AcceptVisitor(IVisitor * visitor) { visitor->Visit(this); }

	Scale() : scale(1.0f) {}
	Scale(float scale) : scale(scale) {}

	float Get() const { return scale; }
	void  Set(float scale) { this->scale = scale; }
};

}}}
