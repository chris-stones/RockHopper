
#include "PlatformHeaders/PlatformHeaders.hpp"

#include <Libs/GLM/glm.hpp>

#include <Platforms/PlatformBootstrapper.hpp>
#include <UserInterface/UserInterface.hpp>
#include <Graphics/Graphics.hpp>

#include <Platforms/Common/UserInterface/SDL/SDLWindowImplementation.hpp>
#include <Platforms/Common/Graphics/OpenGL/Graphics.hpp>
#include <Platforms/Common/Graphics/OpenGL/GraphicsAPI.hpp>

// Initialise window input's event publisher interface.
RH::Libs::EventDispatcher::DirectDispatcher RH::UI::InputSubscriberBase::inputSubscriber;

namespace RH {
	namespace Platform {

		class PlatformBootstrapper::Impl : public RH::Libs::IoCCBase {

			void CreateTextureFactory() {

				std::shared_ptr<::RH::Graphics::TextureFactory> tf =
					std::make_shared<::RH::Graphics::TextureFactory>();

				this->container.Store(tf);
			}

		public:
			Impl() {

				glewInit();
				CreateTextureFactory();
			}
		};

		PlatformBootstrapper::PlatformBootstrapper() {

			impl = new Impl();
		}

		PlatformBootstrapper::~PlatformBootstrapper() {

			delete impl;
		}

	}
}

