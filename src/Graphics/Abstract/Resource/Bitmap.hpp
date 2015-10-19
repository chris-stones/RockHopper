
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Bitmap
	:	public IResource
{
public:

	Bitmap(const char * resource, std::shared_ptr<TextureFactory> tf);
	virtual ~Bitmap();

	// IResource
	float GetWidth() const;
	float GetHeight() const;
	void ReadCoords(int dim, int stride, float * coords) const;

	virtual void AcceptVisitor(IVisitor * visitor) {
		visitor->Visit(this);
	}

	class Impl;
	Impl * GetImpl() { return impl; }

private:

	Impl * impl;
};

}}}

