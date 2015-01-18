
#pragma once

#include <memory>
#include "WindowHints.hpp"

namespace RH { namespace UI {

class Window {

public:
	Window(const WindowHints &hints);
	Window();
	~Window();

	void DispatchInputEvents();

public:
	class Impl;

	Impl * GetImpl() { return impl; }

private:
	Impl * impl;
};

}}

