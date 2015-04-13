
#pragma once

#include<Libs/EventDispatcher/EventDispatcher.hpp>
#include "Events/KeyEvents.hpp"

namespace RH { namespace UI {

// Classes that wish to subscribe to input events should inherit 'InputSubscriberBase'.
class InputSubscriberBase
{
protected:
  static Libs::EventDispatcher::DirectDispatcher	inputSubscriber;


public:
	typedef  RH::UI::Events::KeyPressedEvent KeyPressedEvent;
	typedef  RH::UI::Events::KeyReleasedEvent KeyReleasedEvent;
};

}}

