
#pragma once

namespace RH { namespace Platform {

	class PlatformBootstrapper {

	public:

		class Impl;

		PlatformBootstrapper();
		~PlatformBootstrapper();

	private:

		Impl * impl;
	};
}}
