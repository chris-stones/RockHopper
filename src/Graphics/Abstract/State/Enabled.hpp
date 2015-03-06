
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Enabled : public Object {

	bool enabled;
public:

	virtual void AcceptVisitor(IVisitor * visitor) { visitor->Visit(this); }

	Enabled() : enabled(true) {}
	Enabled(bool enabled) : enabled(enabled) {}

	bool Get() const { return enabled; }
	void  Set(bool enabled) { this->enabled = enabled; }
};

}}}
