
#pragma once

namespace RH { namespace Graphics {

template<typename _T>
class WidgetBase : public Node {

	std::unique_ptr<_T> obj;

public:

	template<typename... Args>
	WidgetBase(Node * parent, Args&&... args)
		:	Node(parent)
	{
		this->obj = std::unique_ptr<_T>( new _T(args...) );
		MySceneGraphTail()->AddChild( this->obj.get() );
	}

	virtual ~WidgetBase() {

		// Unlink this nodes scene graph branch.
		if(IsNotOrphan())
			MySceneGraphTail()->RemoveChild(obj.get());
	}
};

/*****************************
 * Specialisation for Sprite:
 * 	Position by top-left.
 *****************************/
template<>
class WidgetBase<Abstract::Sprite> : public Node {

	std::unique_ptr<Abstract::Sprite> obj;

public:

	template<typename... Args>
	WidgetBase(Node * parent, Args&&... args)
		:	Node(parent)
	{
		this->obj = std::unique_ptr<Abstract::Sprite>( new Abstract::Sprite(args...) );
		MySceneGraphTail()->AddChild( this->obj.get() );

		this->SetAlignmentCorrection( this->obj->AlignmentCorrection() );
	}

	virtual ~WidgetBase() {

		// Unlink this nodes scene graph branch.
		if(IsNotOrphan())
			MySceneGraphTail()->RemoveChild(obj.get());
	}
};

}}
