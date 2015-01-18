
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Alpha : public Object {

	float alpha;
public:

	virtual void AcceptVisitor(IVisitor * visitor) { visitor->Visit(this); }

	Alpha() : alpha(1.0f) {}
	Alpha(float alpha) : alpha(alpha) {}

	float Get() const { return alpha; }
	void  Set(float alpha) { this->alpha = alpha; }
};

}}}
