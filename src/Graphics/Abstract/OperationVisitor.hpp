
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class OperationVisitor : public IVisitor {

	virtual void Visit(Projection * enabled) {}
	virtual void Visit(Enabled * enabled) {}
	virtual void Visit(Alpha * alpha) {}
	virtual void Visit(Scale * scale) {}
	virtual void Visit(Rotate * rotate) {}
	virtual void Visit(Translate * translate) {}
};

}}}
