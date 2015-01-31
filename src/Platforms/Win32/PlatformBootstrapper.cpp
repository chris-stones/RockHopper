
#include "PlatformHeaders.hpp"

#include <Libs/GLM/glm.hpp>

#include <Platforms/PlatformBootstrapper.hpp>
#include <UserInterface/UserInterface.hpp>
#include <Graphics/Graphics.hpp>

#include "UserInterface/UserInterface.hpp"

#include <Platforms/Common/Graphics/OpenGL/Graphics.hpp>
#include <Platforms/Common/Graphics/OpenGL/GraphicsAPI.hpp>

namespace RH {
	namespace Platform {

		class PlatformBootstrapper::Impl {

			void DistributeTextureFactory() {

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

