
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Hidden : public Object {

	bool hidden;
public:

	virtual void AcceptVisitor(IVisitor * visitor) { visitor->Visit(this); }

	Hidden() : hidden(false) {}
	Hidden(bool hidden) : hidden(hidden) {}

	bool Get() const { return hidden; }
	void  Set(bool enabled) { this->hidden = enabled; }
};

}}}
