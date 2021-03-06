
#pragma once

#include<Libs/Concurrency/IConcurrentJobQueue.hpp>

namespace RH { namespace Graphics { namespace Abstract {

class MotionVideo
	:	public IResource,
	 	public Updatable
{
public:

	MotionVideo(UpdatedNode * parent, const std::string & s, std::shared_ptr<RH::Libs::Concurrency::IConcurrentJobQueue> cjq);
	virtual ~MotionVideo();

	// IResource
	float GetWidth() const override;
	float GetHeight() const override;
	void ReadCoords(int dim, int stride, float * coords) const override;

	// IUpdatable
	void Update() override { NextFrame(); }

	void Play();
	void Reset();
	bool IsFinished() const;

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

