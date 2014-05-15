#ifndef InputManager_h__
#define InputManager_h__

#include <array>

namespace Windows
{
#include <Xinput.h>
#undef min
#undef max
}

#include "EventBroker.h"
#include "Events/KeyDown.h"
#include "Events/KeyUp.h"
#include "Events/MousePress.h"
#include "Events/MouseRelease.h"
#include "Events/MouseMove.h"
#include "Events/GamepadAxis.h"
#include "Events/GamepadButton.h"

class InputManager
{
public:
	InputManager(GLFWwindow* window, std::shared_ptr<EventBroker> eventBroker)
	: m_GLFWWindow(window)
	, m_EventBroker(eventBroker)
	, m_CurrentKeyState()
	, m_LastKeyState()
	, m_CurrentMouseState()
	, m_LastMouseState()
	, m_CurrentMouseX(0), m_CurrentMouseY(0)
	, m_LastMouseX(0), m_LastMouseY(0)
	, m_CurrentMouseDeltaX(0), m_CurrentMouseDeltaY(0) 
	{ Initialize(); }

	void Initialize();

	void Update(double dt);

private:
	GLFWwindow* m_GLFWWindow;
	std::shared_ptr<EventBroker> m_EventBroker;
	
	std::array<int, GLFW_KEY_LAST+1> m_CurrentKeyState;
	std::array<int, GLFW_KEY_LAST+1> m_LastKeyState;
	std::array<int, GLFW_MOUSE_BUTTON_LAST+1> m_CurrentMouseState;
	std::array<int, GLFW_MOUSE_BUTTON_LAST+1> m_LastMouseState;
	typedef std::array<float, static_cast<int>(Gamepad::Axis::LAST) + 1> GamepadAxisState;
	std::array<GamepadAxisState, XUSER_MAX_COUNT> m_CurrentGamepadAxisState;
	std::array<GamepadAxisState, XUSER_MAX_COUNT> m_LastGamepadAxisState;
	typedef std::array<bool, static_cast<int>(Gamepad::Button::LAST) + 1> GamepadButtonState;
	std::array<GamepadButtonState, XUSER_MAX_COUNT> m_CurrentGamepadButtonState;
	std::array<GamepadButtonState, XUSER_MAX_COUNT> m_LastGamepadButtonState;

	double m_CurrentMouseX, m_CurrentMouseY;
	double m_LastMouseX, m_LastMouseY;
	double m_CurrentMouseDeltaX, m_CurrentMouseDeltaY;

	void PublishGamepadAxisIfChanged(int gamepadID, Gamepad::Axis axis);
	void PublishGamepadButtonIfChanged(int gamepadID, Gamepad::Button button);
};

#endif // InputManager_h__
