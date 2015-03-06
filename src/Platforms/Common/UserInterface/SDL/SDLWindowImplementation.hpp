#pragma once

#include <Libs/EventDispatcher/EventDispatcher.hpp>
#include <UserInterface/Window.hpp>
#include <UserInterface/Input.hpp>

#include <SDL.h>

class Input_ {
public:
	static RH::Libs::EventDispatcher::EventPublicationManager eventPublicationManager;
};

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

		
		class Window::Impl {

			SDL_Window * mainWindow{ nullptr };
			SDL_GLContext mainContext{ nullptr };

		public:

			Impl(const WindowHints & wh)
			{
				// FIXME - multiple windows? DONT INITIALISE SDL HERE!
				if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
					throw std::runtime_error("SDL_Init()<0");

				Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
				if (wh.IsFullScreen())
					flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

				mainWindow = SDL_CreateWindow("RockHopper", wh.GetX(), wh.GetY(), wh.GetW(), wh.GetH(), flags);
				if (!mainWindow)
					throw std::runtime_error("SDL_CreateWindow()==nullptr");

				mainContext = SDL_GL_CreateContext(mainWindow);
				if (!mainContext)
					throw std::runtime_error("SDL_GL_CreateContext()==nullptr");

				SDL_GL_SetSwapInterval(1);
			}

			~Impl() {

				// FIXME - multiple windows ? DONT SHUTDOWN SDL HERE!
				SDL_GL_DeleteContext(mainContext);
				SDL_DestroyWindow(mainWindow);
				SDL_Quit();
			}

			void DispatchInputEvents() {

				SDL_Event e;

				while (SDL_PollEvent(&e)) {
					switch (e.type) {
					case SDL_KEYDOWN: break;
					case SDL_KEYUP: break;
					case SDL_MOUSEMOTION: break;
					case SDL_MOUSEBUTTONDOWN: break;
					case SDL_MOUSEBUTTONUP: break;
					}
				}
			}

			void MakeContextCurrent() {

			}

			void SwapBuffers() {

				SDL_GL_SwapWindow(mainWindow);
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

