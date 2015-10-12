#pragma once

/*** https://github.com/chris-stones/IoCC.git ***/
#include <IoCC/IoCC.hpp>

namespace RH { namespace Libs {

	using ::ioc::IoCC;

	class IoCCBase {
	protected:
		static IoCC container;
	public:
		virtual ~IoCCBase() {}
	};
}}
