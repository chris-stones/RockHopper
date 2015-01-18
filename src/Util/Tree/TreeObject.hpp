
#pragma once

#include <list>
#include <algorithm>

namespace RH { namespace Util { namespace Tree {

template<typename _VisitorInterfaceT>
class IAcceptVisitor {
public:
	virtual ~IAcceptVisitor() {}
	virtual void AcceptVisitor(_VisitorInterfaceT * visitor) = 0;
};

template<typename _VisitorInterfaceT>
class TreeObject : public IAcceptVisitor<_VisitorInterfaceT> {

	typedef std::list<TreeObject*> Children;

	Children children;

public:
	virtual ~TreeObject() {}

	virtual void AcceptVisitor(_VisitorInterfaceT * visitor) {

		PropagateToChildren(visitor);
	}

	virtual void PropagateToChildren(_VisitorInterfaceT * visitor) {
		for(auto obj : children)
			obj->AcceptVisitor(visitor);
	}

	void SetOnlyChild( TreeObject * object ) {

		SetChildless();
		children.push_back(object);
	}
	void SetChildless() {

		children.clear();
	}
	void AddChild(TreeObject * object) {

		children.push_back(object);
	}

	void RemoveChild(TreeObject * object) {

		auto itor = std::find(children.begin(), children.end(), object);
		if(itor != children.end())
			children.erase(itor);
	}
};

}}}
