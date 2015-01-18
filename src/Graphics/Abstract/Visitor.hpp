
/*** Abstract visitor interface ***/

#pragma once

namespace RH { namespace Graphics { namespace Abstract {

class Projection;
class Enabled;
class Alpha;
class Scale;
class Rotate;
class Translate;
class Sprite;
class Bitmap;
class MotionVideo;

class IVisitor {
public:
	virtual ~IVisitor(){}

	virtual void Visit(Projection * enabled) = 0;
	virtual void Visit(Enabled * enabled) = 0;
	virtual void Visit(Alpha * alpha) = 0;
	virtual void Visit(Scale * scale) = 0;
	virtual void Visit(Rotate * rotate) = 0;
	virtual void Visit(Translate * translate) = 0;

	virtual void Visit(Sprite * sprite) = 0;

	virtual void Visit(Bitmap * bitmap) {}
	virtual void Visit(MotionVideo * motionVideo) {}
};

}}}


