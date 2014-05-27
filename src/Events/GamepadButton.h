#ifndef Events_GamepadButton_h__
#define Events_GamepadButton_h__

#include "EventBroker.h"

namespace Gamepad
{
	enum class Button
	{
		Up,
		Down,
		Left,
		Right,
		Start,
		Back,
		LeftThumb,
		RightThumb,
		LeftShoulder,
		RightShoulder,
		A,
		B,
		X,
		Y,
		LAST = Y
	};
}

namespace Events
{

struct GamepadButtonDown : Event
{
	int GamepadID;
	Gamepad::Button Button;
};

struct GamepadButtonUp : Event
{
	int GamepadID;
	Gamepad::Button Button;
};

}

#endif // Events_GamepadButton_h__