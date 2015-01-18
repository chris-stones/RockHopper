
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <Platforms/PlatformBootstrapper.hpp>
#include <UserInterface/UserInterface.hpp>
#include <Graphics/Graphics.hpp>

#include "UserInterface/UserInterface.hpp"
#include <Platforms/Common/Graphics/OpenGL/Graphics.hpp>
#include <Platforms/Common/Graphics/OpenGL/GraphicsAPI.hpp>

// Instance of event dispatcher for UI input events.
static RH::Libs::EventDispatcher::Manager inputEventDispatcherManager;

// Initialise window input's event publisher interface.
//RH::Libs::EventDispatcher::EventPublicationManager Input_::eventPublicationManager = inputEventDispatcherManager.PublicationInterface();

// Initialise UI's event subscriber / scheduler interface.
RH::Libs::EventDispatcher::EventSubscriptionManager RH::UI::InputSubscriberBase::inputSubscriber = inputEventDispatcherManager.SubscriptionInterface();
RH::Libs::EventDispatcher::EventScheduleManager RH::UI::InputSubscriberBase::inputScheduler = inputEventDispatcherManager.SchedulerInterface();

namespace RH { namespace Platform {

class PlatformBootstrapper::Impl {

	void DistributeTextureFactory() {

		TextureFactory_ * tf = new TextureFactory_();

		UsesTextureFactory_::SetTextureFactory( tf );
	}

	void DistribureWindowSize() {

		// TODO:
		UsesWindowSize_::SetWindowSize(800.0f, 600.0f);
	}

public:
	Impl() {

		DistribureWindowSize();
		DistributeTextureFactory();
	}
};

PlatformBootstrapper::PlatformBootstrapper() {

	impl = new Impl();
}

PlatformBootstrapper::~PlatformBootstrapper() {

	delete impl;
}

}}

