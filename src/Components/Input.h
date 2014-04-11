#ifndef Components_Input_h__
#define Components_Input_h__

#include <array>

#include <GLFW/glfw3.h>

#include "Component.h"

namespace Components
{

struct Input : Component
{	std::array<int, GLFW_KEY_LAST+1> KeyState;
	std::array<int, GLFW_KEY_LAST+1> LastKeyState;
	std::array<int, GLFW_MOUSE_BUTTON_LAST+1> MouseState;
	std::array<int, GLFW_MOUSE_BUTTON_LAST+1> LastMouseState;
	float dX, dY;
	float WheelDelta;
};

}

#endif // !Components_Input_h__