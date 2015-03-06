
#pragma once

#include <Libs/EventDispatcher/EventDispatcher.hpp>
#include <UserInterface/Window.hpp>
#include <UserInterface/Input.hpp>

#include "API/Win32Window.hpp"
#include "API/WGL/WGLContext.hpp"
#include "API/Win32Input.hpp"

namespace RH {
	namespace UI {

		// Default window hints.
		static const WindowHints & _getDefaultWindowHints() {

			static const WindowHints defaultWindowHints(
				WindowHints::DECORATIONS |
				WindowHints::CURSOR |
				WindowHints::KEY_EVENTS |
				WindowHints::CLICK_EVENTS |
				WindowHints::MOUSEMOVE_EVENTS,
				0, 0,
				800, 600
				);

			return defaultWindowHints;
		}

		// Window implementation for Win32 (OpenGL)
		class Window::Impl {

			std::shared_ptr<Win32Window_> w32Window;
			std::shared_ptr<WGLContext_> wglContext;
			std::shared_ptr<Win32Input_> win32Input;

		public:

			Impl(const WindowHints & wh) {

				w32Window = std::make_shared<Win32Window_>(_getDefaultWindowHints());
				wglContext = std::make_shared<WGLContext_>(w32Window->GetHWND());
				win32Input = std::make_shared<Win32Input_>();

				MakeContextCurrent();
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			void MakeContextCurrent() {

				wglContext->MakeContextCurrent();
			}

			void DispatchInputEvents() {

				win32Input->DispatchInputEvents();
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

	}
}