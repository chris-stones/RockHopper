
#pragma once

#if(HAVE_GLX)

#include <memory>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <Libs/EventDispatcher/EventDispatcher.hpp>
#include <UserInterface/Window.hpp>
#include <UserInterface/Input.hpp>

#include "API/X11Display.hpp"
#include "API/X11Window.hpp"
#include "API/GLX/GLXScreen.hpp"
#include "API/GLX/GLXWindow.hpp"
#include "API/X11Input.hpp"
#include "API/GLX/GLXContext.hpp"

namespace RH { namespace UI {

// Default window hints.
static const WindowHints & _getDefaultWindowHints() {

	static const WindowHints defaultWindowHints(
			WindowHints::DECORATIONS |
			WindowHints::CURSOR |
			WindowHints::KEY_EVENTS |
			WindowHints::CLICK_EVENTS |
			WindowHints::MOUSEMOVE_EVENTS,
			0,0,
			1920,1080
	);

	return defaultWindowHints;
}

// Window implementation for GNU/Linux
class Window::Impl {

	std::shared_ptr<X11Dpy_> 	display;
	std::shared_ptr<GLXScreen_>	screen;
	std::unique_ptr<GLXWindow_>	window;
	std::unique_ptr<GLX_>		ctx;
	std::unique_ptr<Input_>		input;

public:

	Impl(const WindowHints & wh) {

		display = std::shared_ptr<X11Dpy_>(new X11Dpy_());
		screen  = std::shared_ptr<GLXScreen_>( new GLXScreen_(display, 0) );

		window  = std::unique_ptr<GLXWindow_>(new GLXWindow_(
				wh,
				display->Get(),
				screen->GetXVisualInfo(),
				screen->GetGLXFBConfig()[0]));

		ctx		= std::unique_ptr<GLX_>( new GLX_(screen, 2,1, NULL) );

		input   = std::unique_ptr<Input_>( new Input_(
				display->Get(),
				window->GetX11Window(),
				window->GetX11EventsMask()));

		MakeContextCurrent();

		glEnable(GL_BLEND);
		glBlendEquation( GL_FUNC_ADD );
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}

	void DispatchInputEvents() {

		input->DispatchInputEvents();
	}

	void MakeContextCurrent() {

		glXMakeContextCurrent(
			display->Get(),
			window->GetGLXWindow(),
			window->GetGLXWindow(),
			ctx->Get());
	}

	void SwapBuffers() {

		glXSwapBuffers( display->Get(), window->GetGLXWindow() );
	}
};

Window::Window() {

	impl = new Impl(_getDefaultWindowHints());
}

Window::Window(const WindowHints & wh) {

	impl = new Impl(wh);
}

Window::~Window() {

	delete impl;
}

void Window::DispatchInputEvents() {

	impl->DispatchInputEvents();
}

} }

#endif /*** HAVE_GLX ***/
