
#include "PlatformHeaders/PlatformHeaders.hpp"

#include <Libs/GLM/glm.hpp>

#include <Platforms/PlatformBootstrapper.hpp>
#include <UserInterface/UserInterface.hpp>
#include <Graphics/Graphics.hpp>

#include <Platforms/Common/UserInterface/SDL/SDLWindowImplementation.hpp>
#include <Platforms/Common/Graphics/OpenGL/Graphics.hpp>
#include <Platforms/Common/Graphics/OpenGL/GraphicsAPI.hpp>

// Instance of event dispatcher for UI input events.
static RH::Libs::EventDispatcher::Manager inputEventDispatcherManager;

// Initialise window input's event publisher interface.
RH::Libs::EventDispatcher::EventPublicationManager Input_::eventPublicationManager = inputEventDispatcherManager.PublicationInterface();

// Initialise UI's event subscriber / scheduler interface.
RH::Libs::EventDispatcher::EventSubscriptionManager RH::UI::InputSubscriberBase::inputSubscriber = inputEventDispatcherManager.SubscriptionInterface();
RH::Libs::EventDispatcher::EventScheduleManager RH::UI::InputSubscriberBase::inputScheduler = inputEventDispatcherManager.SchedulerInterface();

namespace RH {
	namespace Platform {

		class PlatformBootstrapper::Impl {

			void DistributeTextureFactory() {

				glewInit();

				TextureFactory_ * tf = new TextureFactory_();

				UsesTextureFactory_::SetTextureFactory(tf);
			}

		public:
			Impl() {

				DistributeTextureFactory();
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

