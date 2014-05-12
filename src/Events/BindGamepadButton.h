#ifndef Events_BindGamepadButton_h__
#define Events_BindGamepadButton_h__

#include <boost/any.hpp>

#include "EventBroker.h"
#include "Events/GamepadButton.h"

namespace Events
{

struct BindGamepadButton : Event
{
	Gamepad::Button Button;
	std::string Command;
	float Value;
};

}

#endif // Events_BindGamepadButton_h__