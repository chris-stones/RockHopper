
#pragma once

#include<memory>

namespace RH { namespace Graphics {

class IUpdatable {
public:
	virtual ~IUpdatable() {}
	virtual void Update() = 0;
};

class UpdatedNode : public Node {

	// NOTE: modified during iteration.
	//	Must use a container that does not invalidate iterators on insertion.
	typedef std::list<UpdatedNode*> UpdatedNodeContainer;
	typedef std::list<IUpdatable*>	UpdatableContainer;
	////////////////////////////////////////

	UpdatedNodeContainer updatedChildren;
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
		UpdatedNode * un = GetFirstAncestorOfType<UpdatedNode>();
		if(un)
			un->updatedChildren.push_back(this);
	}

	virtual ~UpdatedNode() {

		UpdatedNode * un = GetFirstAncestorOfType<UpdatedNode>();

		if(un) {
			// Unregister this Node with parent
			auto itor = std::find(un->updatedChildren.begin(), un->updatedChildren.end(), this);
			if( itor != un->updatedChildren.end())
				*itor = nullptr; // DON'T erase from container. UpdateAll() may be iterating over it!
		}
	}

private:

	void Update( UpdatableContainer::value_type & value ) {

		if(value)
			value->Update();
	}

	void Update(UpdatedNodeContainer::value_type & value) {

		if(value)
			value->UpdateAll();
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
	void UpdateAll() {

		UpdateInContainer( updatables );
		UpdateInContainer( updatedChildren );
	}
};

class Updatable : public IUpdatable
{
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
