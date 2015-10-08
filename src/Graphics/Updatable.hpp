
#pragma once

#include<memory>

#include "Node.hpp"

namespace RH { namespace Graphics {

class IUpdatable {
public:
	virtual ~IUpdatable() {}
	virtual void Update() = 0;
};

class UpdatedNode
	: public Node,
	  public IUpdatedNode
{
	// NOTE: modified during iteration.
	//	Must use a container that does not invalidate iterators on insertion.
	typedef std::list<IUpdatable*>	UpdatableContainer;
	////////////////////////////////////////

	UpdatableContainer updatables;

public:

	//////////////////// FUNCTIONS AVAILABLE TO ONLY 'class Updatable' ////////////////
	struct Keys {
		class MaintainUpdatables {
			friend class Updatable;
			MaintainUpdatables(){}
		};
	};
	void AddUpdatable(const Keys::MaintainUpdatables&, IUpdatable * updatable) {

		updatables.push_back(updatable);
	}
	void RemoveUpdatable(const Keys::MaintainUpdatables&, IUpdatable * updatable) {

		std::list<IUpdatable*>::iterator itor =
			std::find(updatables.begin(), updatables.end(), updatable);

		if(itor != updatables.end())
			*itor = nullptr; // DON'T erase from container. UpdateAll() may be iterating over it!
	}
	////////////////////////////////////////////////////////////////////////////////////

public:

	UpdatedNode(Node * node)
		:	Node(node)
	{
		this->SetUpdatedNode(this);
	}

	virtual ~UpdatedNode() {

	}

private:

	void Update( UpdatableContainer::value_type & value ) {

		if(value)
			value->Update();
	}

	template<typename _T>
	void EraseNullsFromContainer(_T & container) {

		container.erase(
			std::remove(
				container.begin(),
				container.end(),
				nullptr  ),
			container.end());
	}

	template<typename _T>
	void UpdateInContainer(_T & t) {

		bool hasNulls = false;
		for(auto i : t) {
			if(!i)
				hasNulls = true;
			Update(i);
		}
		if(hasNulls)
			EraseNullsFromContainer(t);
	}

public:
	virtual void UpdateNode() override {

		UpdateInContainer( updatables );
	}
};

class Updatable : public IUpdatable
{
protected:
	UpdatedNode * parent;

public:

	Updatable( UpdatedNode * parent )
		:	parent(parent)
	{
		parent->AddUpdatable( UpdatedNode::Keys::MaintainUpdatables(), this );
	}
	virtual ~Updatable() {

		parent->RemoveUpdatable( UpdatedNode::Keys::MaintainUpdatables(), this );
	}
};

}}
