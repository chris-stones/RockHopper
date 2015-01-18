
#pragma once

namespace RH { namespace Graphics {

class Renderer
	:	public Abstract::IVisitor
{
	typedef std::stack<float> 		AlphaStack;
	typedef std::stack<glm::mat4> 	TranslateStack;

	AlphaStack		alphaStack;
	TranslateStack	translateStack;

	template<typename _T>
	class Stack {
		_T & stack;
	public:
		template<typename _Obj>
		Stack(_T & stack, const _Obj & object) : stack(stack) {

			stack.push(object);
		}
		~Stack() {
			stack.pop();
		}
	};

	GraphicsAPI * api;

public:

	Renderer(GraphicsAPI * api)
		:	api(api)
	{
		alphaStack.push(1.0f);
		translateStack.push(glm::mat4(1.0f));
	}

	void Render(Node * root, UI::Window * window)
	{
		api->Begin(window);
		root->SceneGraph()->AcceptVisitor(this);
		api->End();
	}

	virtual void Visit(Abstract::Enabled * object) {

		if(object->Get())
			object->PropagateToChildren(this);
	}
	virtual void Visit(Abstract::Alpha * object) {

		Stack<AlphaStack> stackOp(alphaStack,
			alphaStack.top() * object->Get() );

		object->PropagateToChildren(this);
	}
	virtual void Visit(Abstract::Scale * object) {

		Stack<TranslateStack> stackOp(translateStack,
			glm::scale(translateStack.top(), glm::vec3(object->Get())));

		object->PropagateToChildren(this);
	}
	virtual void Visit(Abstract::Rotate * object) {

		Stack<TranslateStack> stackOp(translateStack,
			object->Get() * translateStack.top());

		object->PropagateToChildren(this);
	}
	virtual void Visit(Abstract::Translate * object) {

		Stack<TranslateStack> stackOp(translateStack,
			glm::translate( translateStack.top(), object->Get()));

		object->PropagateToChildren(this);
	}
	virtual void Visit(Abstract::Projection * object) {

		Stack<TranslateStack> stackOp(translateStack,
			object->Get() * translateStack.top());

		object->PropagateToChildren(this);
	}
	virtual void Visit(Abstract::Sprite * object) {

		api->Render(object, translateStack.top(), alphaStack.top() );

		object->PropagateToChildren(this);
	}
};

}}

