
#pragma once

#include <UserInterface/Window.hpp>

#ifdef HAVE_EGL_EGL_H
	#include <EGL/egl.h>
#endif

#ifdef HAVE_GLES2_GL2_H
	#include <GLES2/gl2.h>
#endif

#ifdef HAVE_GLES2_GL2EXT_H
	#include <GLES2/gl2ext.h>
#endif

#include <bcm_host.h>
#include <memory>

#include "API/EGLDisplay.hpp" // Very RaspberryPi specialised EGLDisplay.
#include <Platforms/Common/Graphics/EGL/EGLScreen.hpp>
#include <Platforms/Common/Graphics/EGL/EGLWindow.hpp>
#include <Platforms/Common/Graphics/EGL/EGLContext.hpp>

#include <Libs/EventDispatcher/EventDispatcher.hpp>
#include <UserInterface/Window.hpp>
#include <UserInterface/Input.hpp>

namespace RH { namespace UI {

// Default window hints.
static const WindowHints & _getDefaultWindowHints() {

	static const WindowHints defaultWindowHints(
			0,
			0,0,
			800,600
	);

	return defaultWindowHints;
}

// Window implementation for GNU/Linux GLX
class Window::Impl {

	std::unique_ptr<Display_> display;
	std::unique_ptr<Screen_>  screen;
	std::unique_ptr<Window_>  window;
	std::unique_ptr<EGL_> context;

public:
	Impl(const WindowHints & wh) {

		display = std::unique_ptr<Display_> ( new Display_() );
		screen  = std::unique_ptr<Screen_ > ( new Screen_ (display->Get())  );
		window  = std::unique_ptr<Window_ > ( new Window_ (display->Get(), screen->GetConfig(), display->GetNativeWindow()) );
		context = std::unique_ptr<EGL_> ( new EGL_(display->Get(), screen->GetConfig()) );

		MakeContextCurrent();

		glEnable(GL_BLEND);
		glBlendEquation( GL_FUNC_ADD );
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}

	void MakeContextCurrent() {

		eglMakeCurrent(
			display->Get(),
			window->GetSurface(),
			window->GetSurface(),
			context->GetContext());
	}

	void SwapBuffers() {

		eglSwapBuffers (
			display->Get(),
			window->GetSurface());
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

}

}}
