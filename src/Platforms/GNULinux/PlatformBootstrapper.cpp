
#ifdef HAVE_CONFIG_H
	#include <config.h>
#else
	// NOT an auto-tools build.
	// define some sane defaults.
	#define HAVE_X11_XLIB_H 1

//	#define HAVE_GLX 1
	#define HAVE_EGL 1

	#if(HAVE_GLX)
		#define HAVE_GL_GLX_H 1
		#define HAVE_GL_GLXEXT_H 1
		#define HAVE_GL_GL_H 1
		#define HAVE_GL_GLEXT_H 1
	#endif

	#if(HAVE_EGL)
		#define HAVE_EGL_EGL_H 1
		#define HAVE_GLES2_GL2_H 1
		#define HAVE_GLES2_GL2EXT_H 1
	#endif
#endif

#include "PlatformHeaders.hpp"

#include <Libs/GLM/glm.hpp>

#include <Platforms/PlatformBootstrapper.hpp>
#include <UserInterface/UserInterface.hpp>
#include <Graphics/Graphics.hpp>

#include "UserInterface/UserInterface.hpp"
#include <Platforms/Common/Graphics/OpenGL/Graphics.hpp>
#include <Platforms/Common/Graphics/OpenGL/GraphicsAPI.hpp>

// Initialise window input's event publisher interface.
RH::Libs::EventDispatcher::DirectDispatcher RH::UI::InputSubscriberBase::inputSubscriber;

namespace RH { namespace Platform {

class PlatformBootstrapper::Impl {

	void DistributeTextureFactory() {

		TextureFactory_ * tf = new TextureFactory_();

		UsesTextureFactory_::SetTextureFactory( tf );
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

}}

