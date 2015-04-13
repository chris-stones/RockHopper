#pragma once

#include <Libs/EventDispatcher/EventDispatcher.hpp>
#include <UserInterface/Window.hpp>
#include <UserInterface/Input.hpp>

#include <SDL.h>

class Input_ : public RH::UI::InputSubscriberBase {

	RH::UI::Events::keys ToKey(SDL_Keycode keyCode) const {

		switch (keyCode){
		default:			return RH::UI::Events::KEY_UNKNOWN;
		case SDLK_ESCAPE:	return RH::UI::Events::KEY_ESCAPE;
		case SDLK_RETURN:	return RH::UI::Events::KEY_RETURN;
		case SDLK_SPACE:	return RH::UI::Events::KEY_SPACE;
		case SDLK_0:		return RH::UI::Events::KEY_0;
		case SDLK_1:		return RH::UI::Events::KEY_1;
		case SDLK_2:		return RH::UI::Events::KEY_2;
		case SDLK_3:		return RH::UI::Events::KEY_3;
		case SDLK_4:		return RH::UI::Events::KEY_4;
		case SDLK_5:		return RH::UI::Events::KEY_5;
		case SDLK_6:		return RH::UI::Events::KEY_6;
		case SDLK_7:		return RH::UI::Events::KEY_7;
		case SDLK_8:		return RH::UI::Events::KEY_8;
		case SDLK_9:		return RH::UI::Events::KEY_9;
		case SDLK_a:		return RH::UI::Events::KEY_A;
		case SDLK_b:		return RH::UI::Events::KEY_B;
		case SDLK_c:		return RH::UI::Events::KEY_C;
		case SDLK_d:		return RH::UI::Events::KEY_D;
		case SDLK_e:		return RH::UI::Events::KEY_E;
		case SDLK_f:		return RH::UI::Events::KEY_F;
		case SDLK_g:		return RH::UI::Events::KEY_G;
		case SDLK_h:		return RH::UI::Events::KEY_H;
		case SDLK_i:		return RH::UI::Events::KEY_I;
		case SDLK_j:		return RH::UI::Events::KEY_J;
		case SDLK_k:		return RH::UI::Events::KEY_K;
		case SDLK_l:		return RH::UI::Events::KEY_L;
		case SDLK_m:		return RH::UI::Events::KEY_M;
		case SDLK_n:		return RH::UI::Events::KEY_N;
		case SDLK_o:		return RH::UI::Events::KEY_O;
		case SDLK_p:		return RH::UI::Events::KEY_P;
		case SDLK_q:		return RH::UI::Events::KEY_Q;
		case SDLK_r:		return RH::UI::Events::KEY_R;
		case SDLK_s:		return RH::UI::Events::KEY_S;
		case SDLK_t:		return RH::UI::Events::KEY_T;
		case SDLK_u:		return RH::UI::Events::KEY_U;
		case SDLK_v:		return RH::UI::Events::KEY_V;
		case SDLK_w:		return RH::UI::Events::KEY_W;
		case SDLK_x:		return RH::UI::Events::KEY_X;
		case SDLK_y:		return RH::UI::Events::KEY_Y;
		case SDLK_z:		return RH::UI::Events::KEY_Z;
		}
	}

public:
	void RaiseKeyPressed(SDL_Keycode keyCode) {

		inputSubscriber.Raise(
			RH::UI::Events::KeyPressedEvent(ToKey(keyCode)));
	}

	void RaiseKeyReleased(SDL_Keycode keyCode) {

		inputSubscriber.Raise(
			RH::UI::Events::KeyReleasedEvent(ToKey(keyCode)));
	}

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
				SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				800, 600
				);

			return defaultWindowHints;
		}

		
		class Window::Impl {

			SDL_Window * mainWindow{ nullptr };
			SDL_GLContext mainContext{ nullptr };
			std::unique_ptr<Input_> input;

		public:

			Impl(const WindowHints & wh)
			{
				// FIXME - multiple windows? DONT INITIALISE SDL HERE!
				if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
					throw std::runtime_error("SDL_Init()<0");

				Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
				if (wh.IsFullScreen())
					flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

				mainWindow = SDL_CreateWindow("RockHopper", 
					wh.GetX(),
					wh.GetY(),
					wh.GetW(), wh.GetH(), flags);
				if (!mainWindow)
					throw std::runtime_error("SDL_CreateWindow()==nullptr");

				mainContext = SDL_GL_CreateContext(mainWindow);
				if (!mainContext)
					throw std::runtime_error("SDL_GL_CreateContext()==nullptr");

				SDL_GL_SetSwapInterval(1);

				input = std::unique_ptr<Input_>(new Input_());
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
					case SDL_KEYDOWN: 
						input->RaiseKeyPressed(e.key.keysym.sym);
						break;
					case SDL_KEYUP: 
						input->RaiseKeyReleased(e.key.keysym.sym);
						break;
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

