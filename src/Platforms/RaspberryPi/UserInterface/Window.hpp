
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

#ifdef HAVE_BCM_HOST_H
#include <bcm_host.h>
#endif

#include <memory>

#include <Platforms/RaspberryPi/UserInterface/API/EGL_Pi_Display.hpp> // Very RaspberryPi specialised EGLDisplay.
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

// Window implementation for RaspberryPi
class Window::Impl {

	std::unique_ptr<EGL_Pi_Display_> display;
	std::unique_ptr<EGLScreen_>      screen;
	std::unique_ptr<EGLWindow_>      window;
	std::unique_ptr<EGL_>            context;

public:
	Impl(const WindowHints & wh) {

		display = std::unique_ptr<EGL_Pi_Display_> ( new EGL_Pi_Display_() );
		screen  = std::unique_ptr<EGLScreen_>      ( new EGLScreen_     (display->Get())  );
		window  = std::unique_ptr<EGLWindow_>      ( new EGLWindow_     (display->Get(), screen->GetConfig(), display->GetNativeWindow()) );
		context = std::unique_ptr<EGL_>            ( new EGL_           (display->Get(), screen->GetConfig()) );

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
