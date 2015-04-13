
/*
 * Checkout: https://github.com/chris-stones/EventDispatcher.git
 */

#pragma once

#include <EventDispatcher/EventDispatcher.hpp>

namespace RH { namespace Libs { namespace EventDispatcher {

	using ::EventDispatcher::ISubscription;
	using ::EventDispatcher::DirectDispatcher;
	using ::EventDispatcher::PipedDispatcher;
	using ::EventDispatcher::BlockableDispatcher;
	using ::EventDispatcher::ScopedBlock;

}}}

