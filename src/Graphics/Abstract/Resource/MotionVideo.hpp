
#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class MotionVideo
	:	public IResource,
	 	public Updatable
{
public:

	MotionVideo(UpdatedNode * parent, const std::string & s);
	virtual ~MotionVideo();

	// IResource
	float GetWidth() const;
	float GetHeight() const;
	void ReadCoords(int dim, int stride, float * coords) const;

	// IUpdatable
	void Update() { NextFrame(); }

	virtual void AcceptVisitor(IVisitor * visitor) {
		visitor->Visit(this);
	}

	class Impl;
	Impl * GetImpl() { return impl; }

private:

	void NextFrame();

	Impl * impl;
};

}}}

