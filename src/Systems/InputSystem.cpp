#include "PrecompiledHeader.h"
#include "InputSystem.h"
#include "World.h"

void Systems::InputSystem::Update(double dt)
{	m_LastKeyState = m_CurrentKeyState;
	m_LastMouseState = m_CurrentMouseState;

	// Keyboard input
	for (int i = 0; i <= GLFW_KEY_LAST; ++i)
	{	m_CurrentKeyState[i] = glfwGetKey(m_Renderer->GetWindow(), i);
	}

	// Mouse buttons
	for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
	{	m_CurrentMouseState[i] = glfwGetMouseButton(m_Renderer->GetWindow(), i);
	}

	// Cursor position
	double xpos, ypos;
	glfwGetCursorPos(m_Renderer->GetWindow(), &xpos, &ypos);
	m_CurrentMouseDeltaX = xpos - m_LastMouseX;
	m_CurrentMouseDeltaY = ypos - m_LastMouseY;
	m_LastMouseX = xpos;
	m_LastMouseY = ypos;

	// Lock mouse while holding LMB
	if (m_CurrentMouseState[GLFW_MOUSE_BUTTON_LEFT])
	{	m_LastMouseX = m_Renderer->WIDTH / 2.f; // xpos;
		m_LastMouseY = m_Renderer->HEIGHT / 2.f; // ypos;
		glfwSetCursorPos(m_Renderer->GetWindow(), m_LastMouseX, m_LastMouseY);
	}
	// Hide/show cursor with LMB
	if (m_CurrentMouseState[GLFW_MOUSE_BUTTON_LEFT] && !m_LastMouseState[GLFW_MOUSE_BUTTON_LEFT])
	{	glfwSetInputMode(m_Renderer->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	if (!m_CurrentMouseState[GLFW_MOUSE_BUTTON_LEFT] && m_LastMouseState[GLFW_MOUSE_BUTTON_LEFT])
	{	glfwSetInputMode(m_Renderer->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

#ifdef DEBUG
	// Wireframe
	if (m_CurrentKeyState[GLFW_KEY_F1] && !m_LastKeyState[GLFW_KEY_F1])
	{	m_Renderer->DrawWireframe(!m_Renderer->DrawWireframe());
	}
	// Normals
	if (m_CurrentKeyState[GLFW_KEY_F2] && !m_LastKeyState[GLFW_KEY_F2])
	{	m_Renderer->DrawNormals(!m_Renderer->DrawNormals());
	}
	// Bounds
	if (m_CurrentKeyState[GLFW_KEY_F3] && !m_LastKeyState[GLFW_KEY_F3])
	{	m_Renderer->DrawBounds(!m_Renderer->DrawBounds());
	}
#endif
}

void Systems::InputSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{	auto input = m_World->GetComponent<Components::Input>(entity, "Input");
	if (input == nullptr)
		return;

	input->KeyState = m_CurrentKeyState;
	input->LastKeyState = m_LastKeyState;
	input->MouseState = m_CurrentMouseState;
	input->LastMouseState = m_LastMouseState;
	input->dX = m_CurrentMouseDeltaX;
	input->dY = m_CurrentMouseDeltaY;
}

std::array<int, GLFW_KEY_LAST+1> Systems::InputSystem::m_CurrentKeyState;
std::array<int, GLFW_KEY_LAST+1> Systems::InputSystem::m_LastKeyState;
