
#pragma once

#include <UserInterface/Window.hpp>

#include <memory>

#include <Platforms/Mali400/UserInterface/API/EGL_Mali400_Display.hpp> // Very Mali400 specialised EGLDisplay.
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
			1920,1080
	);

	return defaultWindowHints;
}

// Window implementation for Mali400
class Window::Impl {

	std::unique_ptr<EGL_Mali400_Display_> display;
	std::unique_ptr<EGLScreen_>      screen;
	std::unique_ptr<EGLWindow_>      window;
	std::unique_ptr<EGL_>            context;

public:
	Impl(const WindowHints & wh) {

		display = std::unique_ptr<EGL_Mali400_Display_> ( new EGL_Mali400_Display_() );
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
