#ifndef Events_BindGamepadAxis_h__
#define Events_BindGamepadAxis_h__

#include <boost/any.hpp>

#include "EventBroker.h"
#include "Events/GamepadAxis.h"

namespace Events
{

struct BindGamepadAxis : Event
{
	Gamepad::Axis Axis;
	std::string Command;
	float Value;
};

}

#endif // Events_BindGamepadAxis_h__