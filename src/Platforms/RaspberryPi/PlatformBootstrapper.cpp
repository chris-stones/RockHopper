
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "PlatformHeaders.hpp"

#include <Platforms/PlatformBootstrapper.hpp>
#include <UserInterface/UserInterface.hpp>
#include <Graphics/Graphics.hpp>

#include <Platforms/RaspberryPi/UserInterface/UserInterface.hpp>
#include <Platforms/Common/Graphics/OpenGL/Graphics.hpp>
#include <Platforms/Common/Graphics/OpenGL/GraphicsAPI.hpp>

#include <Libs/EventDispatcher/EventDispatcher.hpp>

// Initialise window input's event publisher interface.
RH::Libs::EventDispatcher::DirectDispatcher RH::UI::InputSubscriberBase::inputSubscriber;

namespace RH { namespace Platform {

class PlatformBootstrapper::Impl : public RH::Libs::IoCCBase {

	void CreateTextureFactory() {

		std::shared_ptr<::RH::Graphics::TextureFactory> tf =
			std::make_shared<::RH::Graphics::TextureFactory>();

		this->container.Store(tf);
	}

public:
	Impl() {

		CreateTextureFactory();
	}
};

PlatformBootstrapper::PlatformBootstrapper() {

	impl = new Impl();
}

PlatformBootstrapper::~PlatformBootstrapper() {

	delete impl;
}

}}

