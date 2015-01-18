
#pragma once

#if(HAVE_EGL)

#include <memory>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <Libs/EventDispatcher/EventDispatcher.hpp>
#include <UserInterface/Window.hpp>
#include <UserInterface/Input.hpp>

#include <Platforms/Common/Graphics/EGL/EGLDisplay.hpp>
#include <Platforms/Common/Graphics/EGL/EGLScreen.hpp>
#include <Platforms/Common/Graphics/EGL/EGLWindow.hpp>
#include <Platforms/Common/Graphics/EGL/EGLContext.hpp>

#include "API/X11Display.hpp"
#include "API/X11Window.hpp"
#include "API/X11Input.hpp"

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
			800,600
	);

	return defaultWindowHints;
}

// Window implementation for GNU/Linux
class Window::Impl {

	std::shared_ptr<X11Dpy_>	 x11Display;
	std::shared_ptr<EGLDisplay_> eglDisplay;
	std::shared_ptr<EGLScreen_>  eglScreen;
	std::shared_ptr<X11Window_>	 x11Window;
	std::shared_ptr<EGLWindow_>  eglWindow;
	std::shared_ptr<EGL_>		 eglContext;
	std::shared_ptr<Input_>	 	 x11Input;

public:

	Impl(const WindowHints & wh) {

		x11Display = std::make_shared<X11Dpy_>();
		eglDisplay = std::make_shared<EGLDisplay_>(EGL_OPENGL_ES_API, x11Display->Get());
		eglScreen  = std::make_shared<EGLScreen_>(eglDisplay->Get());

		eglContext = std::make_shared<EGL_>(
			eglDisplay->Get(),
			eglScreen->GetConfig());

		int numberOfVisuals;
		XVisualInfo visTemplate;
		visTemplate.visualid = eglScreen->GetVisualID();
		XVisualInfo * xVisInfo = XGetVisualInfo(
			x11Display->Get(),
			VisualIDMask,
			&visTemplate,
			&numberOfVisuals);

		x11Window  = std::make_shared<X11Window_>(_getDefaultWindowHints(), x11Display->Get(), xVisInfo);

		XFree(xVisInfo);

		eglWindow = std::make_shared<EGLWindow_>(
			eglDisplay->Get(),
			eglScreen->GetConfig(),
			x11Window->GetX11Window());

		MakeContextCurrent();

		x11Input = std::make_shared<Input_>(
			x11Display->Get(),
			x11Window->GetX11Window(),
			x11Window->GetX11EventsMask());

		glEnable(GL_BLEND);
		glBlendEquation( GL_FUNC_ADD );
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}

	void DispatchInputEvents() {

		x11Input->DispatchInputEvents();
	}

	void MakeContextCurrent() {

		eglMakeCurrent(
			eglDisplay->Get(),
			eglWindow->GetSurface(),
			eglWindow->GetSurface(),
			eglContext->GetContext());
	}

	void SwapBuffers() {

		eglSwapBuffers (
			eglDisplay->Get(),
			eglWindow->GetSurface());
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

#endif /*** HAVE_EGL ***/
