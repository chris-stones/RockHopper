
#pragma once

#include "Abstract/Object.hpp"
#include "Abstract/State/Alpha.hpp"
#include "Abstract/State/Enabled.hpp"
#include "Abstract/State/Hidden.hpp"
#include "Abstract/State/Update.hpp"
#include "Abstract/State/Rotate.hpp"
#include "Abstract/State/Scale.hpp"
#include "Abstract/State/Translate.hpp"
#include "Abstract/State/Projection.hpp"

#include <memory>
#include <algorithm>
#include <map>
#include <list>
#include <vector>

namespace RH { namespace Graphics {

class Node
{
protected:
	Node * parent;

/*
	template<typename _T>
	_T * GetFirstAncestorOfType() {

		Node * p = this;

		while((p = p->parent)) {

			_T * un;
			if((un = dynamic_cast<_T*>(p)))
				return un;
		}
		return nullptr;
	}
*/

private:
	std::list<Node*> children;

	///// RENDER SCENE GRAPH /////////
	std::unique_ptr<Abstract::Object>     sgHead;		// Top of this nodes scene graph. ( parent references this )
	std::unique_ptr<Abstract::Update>     sgUpdate;     // Optional - update updateable nodes.
	std::unique_ptr<Abstract::Projection> sgProjection; // Optional - new projection matrix.
	std::unique_ptr<Abstract::Enabled> 	  sgEnabled;	// Optional - enable / disable children.
	std::unique_ptr<Abstract::Hidden>     sgHidden;     // Optional - hide / un-hide children. ( update but don't render )
	std::unique_ptr<Abstract::Alpha>      sgAlpha;		// Optional - change alpha of children.
	std::unique_ptr<Abstract::Scale>      sgScale;		// Optional - scale children.
	std::unique_ptr<Abstract::Rotate>     sgRotate;		// Optional - rotate children.
	std::unique_ptr<Abstract::Translate>  sgTranslate;	// Optional - move children.
	std::unique_ptr<Abstract::Object>     sgTail;		// Bottom of this nodes scene graph. ( children referenced to here )
	//////////////////////////////////

	typedef std::map<const char *, std::weak_ptr<Node> > ExportMap;
	ExportMap exports;

public:

	Node(Node* parent)
		:	parent(parent)
	{
		sgHead = std::unique_ptr<Abstract::Object>(new Abstract::Object());
		sgTail = std::unique_ptr<Abstract::Object>(new Abstract::Object());

		if(parent) {

			parent->children.push_back(this);

			parent->sgTail->AddChild(sgHead.get());
		}
		sgHead->AddChild(sgTail.get());
	}

	virtual ~Node() {

		// FIRST, delete all children!
//		for(auto itor = children.begin(); itor != children.end(); itor++) {
//			(*itor)->parent = NULL;
//			delete *itor;
//		}

		// THEN, unlink this from parent.
		if(parent) {

			// Unregister this Node with parent
			auto itor = std::find(parent->children.begin(), parent->children.end(), this);
			if( itor != parent->children.end())
				parent->children.erase(itor);

			// Unlink this nodes scene graph branch.
			parent->sgTail->RemoveChild(sgHead.get());
		}
	}

	Abstract::Object * SceneGraph() {

		return sgHead.get();
	}

protected:

	Abstract::Object * MySceneGraphTail( ) {

		return sgTail.get();
	}

	Abstract::Object * MySceneGraphHead() {

		return sgHead.get();
	}

	bool IsOrphan() const {

		return parent ? true : false;
	}

	bool IsNotOrphan() 	const {

		return !IsOrphan();
	}

	void SetAlignmentCorrection(const glm::vec3 &new_alignment_correction ) {

		if(!sgTranslate)
			SetTranslation(glm::vec3());

		// get old translation with old correction.
		glm::vec3 translation = GetTranslation();
		// set new correction.
		this->alignment_correction = new_alignment_correction;
		// reset old translation with new correction.
		SetTranslation(translation);
	}

protected:

	Node * Root() {

		if(parent)
			return parent->Root();

		return this;
	}

	Node * Parent() {

		if(parent)
			return parent;

		throw std::runtime_error("NULL parent"); // TODO: better error string.
	}

	void Export(const char * name, std::shared_ptr<Node> node) {

		exports[name] = node;
	}

	template<typename _T, typename... Args>
	std::shared_ptr<_T> Make(Args&&... args) {

		std::shared_ptr<_T> obj =
			std::make_shared<_T>(args...);

//		std::shared_ptr<IUpdateable> updatable =
//			std::dynamic_pointer_cast<IUpdateable>(obj);
//
//		if(updatable)
//			AddToUpdateList( updatable );

		return obj;
	}

	template<typename _T, typename... Args>
	std::shared_ptr<_T> MakeExported(const char * name, Args&&... args) {

		std::shared_ptr<_T> obj =
			Make<_T>(args...);

		Export(name, obj);

		return obj;
	}

//public:

	template<typename _T = Node>
	std::weak_ptr<_T> Import(const char * name) {


		ExportMap::iterator itor = exports.find(name);
		if( itor != exports.end() )
			if(std::shared_ptr<Node> p = itor->second.lock())
				if(std::shared_ptr<_T> t = std::dynamic_pointer_cast<_T>(p))
					return t;

		// nothing of type _T was exported with that key.
		throw std::runtime_error("Import error"); // TODO: better error string.
	}

private:

	void UpdateSceneGraph() {

		std::vector<Abstract::Object*> obj;
		obj.reserve(10);
		obj.push_back(sgHead.get());
		if(sgEnabled) obj.push_back(sgEnabled.get());
		if(sgUpdate) obj.push_back(sgUpdate.get());
		if(sgHidden) obj.push_back(sgHidden.get());

		if(sgAlpha) obj.push_back(sgAlpha.get());


/*	rotating from corner!
		if(sgRotate) obj.push_back(sgRotate.get());
		if(sgScale) obj.push_back(sgScale.get());
		if(sgTranslate) obj.push_back(sgTranslate.get());
		if(sgProjection) obj.push_back(sgProjection.get());
*/

/* rotating from corner
		if(sgTranslate) obj.push_back(sgTranslate.get());
		if(sgRotate) obj.push_back(sgRotate.get());
		if(sgScale) obj.push_back(sgScale.get());
		if(sgProjection) obj.push_back(sgProjection.get());
*/

		if(sgProjection) obj.push_back(sgProjection.get());
		if(sgScale) obj.push_back(sgScale.get());
		if(sgRotate) obj.push_back(sgRotate.get());
		if(sgTranslate) obj.push_back(sgTranslate.get());



		obj.push_back(sgTail.get());
		for(uint32_t i = 0; i<obj.size()-1; i++)
			obj[i]->SetOnlyChild(obj[i+1]);
	}

	template<typename _T, typename ...Args>
	std::unique_ptr<_T> MkSg(Args&&... args) {

		return std::unique_ptr<_T>( new _T(std::move(args)... ) );
	}

	template<typename _T, typename _A>
	void Set(_T & p, _A && a) {

		if(p) p->Set( std::move(a) );
		else  p = MkSg<typename _T::element_type>(a);
		UpdateSceneGraph();
	}

	// 2D nodes will want to be placed on screen relative to their top-lefts.
	glm::vec3 alignment_correction;

protected:
	void SetUpdatedNode(IUpdatedNode* updatedNode) {

		Set(sgUpdate,updatedNode);
	}

public:
	/////////////////////// ENABLE / DISABLE THIS NODE /////////////////////////
	void SetEnabled(bool enabled) {

		Set(sgEnabled,enabled);
	}
	bool IsEnabled() const {

		if(sgEnabled) return sgEnabled->Get();
		return true;
	}

	/////////////////////// HIDE / SHOW THIS NODE /////////////////////////
	void SetHidden(bool enabled) {

		Set(sgHidden,enabled);
	}
	bool IsHidden() const {

		if(sgHidden) return sgHidden->Get();
		return false;
	}

	////////////////////////// GET/SET ALPHA ////////////////////////////
	void SetAlpha(float alpha) {

		Set(sgAlpha,alpha);
	}
	float GetAlpha() const {
		if(sgAlpha) return sgAlpha->Get();
		return 1.0f;
	}

	////////////////////////// GET/SET SCALE ////////////////////////////
	void SetScale(float scale) {

		Set(sgScale,scale);
	}
	float GetScale() const {
		if(sgScale) return sgScale->Get();
		return 1.0f;
	}

	////////////////////////// GET/SET ROTATION /////////////////////////
	void SetRotation(const glm::mat4 & rotation) {

		Set(sgRotate,rotation);
	}
	glm::mat4 GetRotation() const {
		if(sgRotate) return sgRotate->Get();
		return glm::mat4(1.0f);
	}

	///////////////////////// GET/SET TRANSLATION ///////////////////////
	void SetTranslation(const glm::vec3 & translation) {

		Set(sgTranslate, translation + alignment_correction );
	}
	glm::vec3 GetTranslation() const {
		if(sgTranslate) return sgTranslate->Get() - alignment_correction;
		return glm::vec3(0.0f);
	}

	///////////////////////// GET/SET PROJECTION ///////////////////////
	void SetProjection(const glm::mat4 & projection) {

		Set(sgProjection, projection);
	}
	glm::mat4 GetProjection() const {
		if(sgProjection) return sgProjection->Get();
		return glm::mat4();
	}
};

}}

