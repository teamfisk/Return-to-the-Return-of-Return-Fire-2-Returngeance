#ifndef Events_GamepadAxis_h__
#define Events_GamepadAxis_h__

#include "EventBroker.h"

namespace Gamepad
{
	enum class Axis
	{
		LeftX,
		LeftY,
		RightX,
		RightY,
		LeftTrigger,
		RightTrigger,
		LAST = RightTrigger
	};
}

namespace Events
{

struct GamepadAxis : Event
{
	int GamepadID;
	Gamepad::Axis Axis;
	float Value;
};

}

#endif // Events_GamepadAxis_h__