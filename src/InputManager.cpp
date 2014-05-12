#include "PrecompiledHeader.h"
#include "InputManager.h"

void InputManager::Update(double dt)
{
	m_LastKeyState = m_CurrentKeyState;
	m_LastMouseState = m_CurrentMouseState;
	m_LastMouseX = m_CurrentMouseX;
	m_LastMouseY = m_CurrentMouseY;

	// Keyboard input
	for (int i = 0; i <= GLFW_KEY_LAST; ++i)
	{
		m_CurrentKeyState[i] = glfwGetKey(m_GLFWWindow, i);
		if (m_CurrentKeyState[i] != m_LastKeyState[i])
		{
			// Publish key events
			if (m_CurrentKeyState[i])
			{
				Events::KeyDown e;
				e.KeyCode = i;
				m_EventBroker->Publish<Events::KeyDown>(e);
			}
			else
			{
				Events::KeyUp e;
				e.KeyCode = i;
				m_EventBroker->Publish<Events::KeyUp>(e);
			}
		}
	}

	// Mouse buttons
	for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
	{
		m_CurrentMouseState[i] = glfwGetMouseButton(m_GLFWWindow, i);
		if (m_CurrentMouseState[i] != m_LastMouseState[i])
		{
			double x, y;
			glfwGetCursorPos(m_GLFWWindow, &x, &y);
			// Publish mouse button events
			if (m_CurrentMouseState[i])
			{
				Events::MousePress e;
				e.Button = i;
				e.X = x;
				e.Y = y;
				m_EventBroker->Publish<Events::MousePress>(e);
			}
			else
			{
				Events::MouseRelease e;
				e.Button = i;
				e.X = x;
				e.Y = y;
				m_EventBroker->Publish<Events::MouseRelease>(e);
			}
		}
	}

	// Cursor position
	glfwGetCursorPos(m_GLFWWindow, &m_CurrentMouseX, &m_CurrentMouseY);
	m_CurrentMouseDeltaX = m_CurrentMouseX - m_LastMouseX;
	m_CurrentMouseDeltaY = m_CurrentMouseY - m_LastMouseY;
	if (m_CurrentMouseDeltaX != 0 || m_CurrentMouseDeltaY != 0)
	{
		// Publish mouse move events
		Events::MouseMove e;
		e.X = m_CurrentMouseX;
		e.Y = m_CurrentMouseY;
		e.DeltaX = m_CurrentMouseDeltaX;
		e.DeltaY = m_CurrentMouseDeltaY;
		m_EventBroker->Publish<Events::MouseMove>(e);
	}

	// // Lock mouse while holding LMB
	// if (m_CurrentMouseState[GLFW_MOUSE_BUTTON_LEFT])
	// {
	// 	m_LastMouseX = m_Renderer->Width() / 2.f; // xpos;
	// 	m_LastMouseY = m_Renderer->Height() / 2.f; // ypos;
	// 	glfwSetCursorPos(m_GLFWWindow, m_LastMouseX, m_LastMouseY);
	// }
	// // Hide/show cursor with LMB
	// if (m_CurrentMouseState[GLFW_MOUSE_BUTTON_LEFT] && !m_LastMouseState[GLFW_MOUSE_BUTTON_LEFT])
	// {
	// 	glfwSetInputMode(m_GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	// }
	// if (!m_CurrentMouseState[GLFW_MOUSE_BUTTON_LEFT] && m_LastMouseState[GLFW_MOUSE_BUTTON_LEFT])
	// {
	// 	glfwSetInputMode(m_GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// }
}
