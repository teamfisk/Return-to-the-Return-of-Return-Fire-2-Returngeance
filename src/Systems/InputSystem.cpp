#include "PrecompiledHeader.h"
#include "InputSystem.h"
#include "World.h"

void Systems::InputSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("Input", []() { return new Components::Input(); });
}

void Systems::InputSystem::Initialize()
{
	// Subscribe to events
	m_EKeyDown = decltype(m_EKeyDown)(std::bind(&Systems::InputSystem::OnKeyDown, this, std::placeholders::_1));
	EventBroker->Subscribe(m_EKeyDown);
}

void Systems::InputSystem::Update(double dt)
{
// #ifdef DEBUG
// 	// Wireframe
// 	if (m_CurrentKeyState[GLFW_KEY_F1] && !m_LastKeyState[GLFW_KEY_F1])
// 	{
// 		m_Renderer->DrawWireframe(!m_Renderer->DrawWireframe());
// 	}
// 	// Normals
// 	if (m_CurrentKeyState[GLFW_KEY_F2] && !m_LastKeyState[GLFW_KEY_F2])
// 	{
// 		m_Renderer->DrawNormals(!m_Renderer->DrawNormals());
// 	}
// 	// Bounds
// 	if (m_CurrentKeyState[GLFW_KEY_F3] && !m_LastKeyState[GLFW_KEY_F3])
// 	{
// 		m_Renderer->DrawBounds(!m_Renderer->DrawBounds());
// 	}
// #endif
}

bool Systems::InputSystem::OnKeyDown(const Events::KeyDown &event)
{
	return true;
}