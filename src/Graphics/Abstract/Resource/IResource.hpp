
#pragma once

#include <Util/Tree/TreeObject.hpp>

namespace RH { namespace Graphics { namespace Abstract {

class IResource
	: public Util::Tree::IAcceptVisitor<IVisitor>
{
public:
	virtual float GetWidth() const = 0;
	virtual float GetHeight() const = 0;
	virtual void ReadCoords(int dim, int stride, float * coords) const = 0;
};

}}}

