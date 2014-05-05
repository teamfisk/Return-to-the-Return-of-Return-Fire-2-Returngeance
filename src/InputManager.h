#ifndef InputManager_h__
#define InputManager_h__

#include <array>

#include "EventBroker.h"
#include "Events/KeyDown.h"
#include "Events/KeyUp.h"
#include "Events/MousePress.h"
#include "Events/MouseRelease.h"
#include "Events/MouseMove.h"

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
	, m_CurrentMouseDeltaX(0), m_CurrentMouseDeltaY(0) { }

	void Update(double dt);

private:
	GLFWwindow* m_GLFWWindow;
	std::shared_ptr<EventBroker> m_EventBroker;
	
	std::array<int, GLFW_KEY_LAST+1> m_CurrentKeyState;
	std::array<int, GLFW_KEY_LAST+1> m_LastKeyState;
	std::array<int, GLFW_MOUSE_BUTTON_LAST+1> m_CurrentMouseState;
	std::array<int, GLFW_MOUSE_BUTTON_LAST+1> m_LastMouseState;
	double m_CurrentMouseX, m_CurrentMouseY;
	double m_LastMouseX, m_LastMouseY;
	double m_CurrentMouseDeltaX, m_CurrentMouseDeltaY;
};

#endif // InputManager_h__
