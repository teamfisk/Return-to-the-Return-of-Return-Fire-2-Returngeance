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
	EVENT_SUBSCRIBE_MEMBER(m_EKeyDown, &Systems::InputSystem::OnKeyDown)
	EVENT_SUBSCRIBE_MEMBER(m_EKeyUp, &Systems::InputSystem::OnKeyUp)
	EVENT_SUBSCRIBE_MEMBER(m_EMousePress, &Systems::InputSystem::OnMousePress)
	EVENT_SUBSCRIBE_MEMBER(m_EMouseRelease, &Systems::InputSystem::OnMouseRelease)
	EVENT_SUBSCRIBE_MEMBER(m_EBindKey, &Systems::InputSystem::OnBindKey)
	EVENT_SUBSCRIBE_MEMBER(m_EBindMouseButton, &Systems::InputSystem::OnBindMouseButton)
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
	auto bindingIt = m_KeyBindings.find(event.KeyCode);
	if (bindingIt != m_KeyBindings.end())
	{
		PublishCommand(0, bindingIt->second, false);
	}

	return true;
}

bool Systems::InputSystem::OnKeyUp(const Events::KeyUp &event)
{
	auto bindingIt = m_KeyBindings.find(event.KeyCode);
	if (bindingIt != m_KeyBindings.end())
	{
		PublishCommand(0, bindingIt->second, true);
	}

	return true;
}

bool Systems::InputSystem::OnMousePress(const Events::MousePress &event)
{
	auto bindingIt = m_MouseButtonBindings.find(event.Button);
	if (bindingIt != m_MouseButtonBindings.end())
	{
		PublishCommand(0, bindingIt->second, false);
	}

	return true;
}

bool Systems::InputSystem::OnMouseRelease(const Events::MouseRelease &event)
{
	auto bindingIt = m_MouseButtonBindings.find(event.Button);
	if (bindingIt != m_MouseButtonBindings.end())
	{
		PublishCommand(0, bindingIt->second, true);
	}

	return true;
}

bool Systems::InputSystem::OnBindKey(const Events::BindKey &event)
{
	if (event.Command.empty())
	{
		m_KeyBindings.erase(event.KeyCode);
	}
	else
	{
		m_KeyBindings[event.KeyCode] = event.Command;
		LOG_DEBUG("Input: Bound key %c to %s", (char)event.KeyCode, event.Command.c_str());
	}

	return true;
}

bool Systems::InputSystem::OnBindMouseButton(const Events::BindMouseButton &event)
{
	if (event.Command.empty())
	{
		m_MouseButtonBindings.erase(event.Button);
	}
	else
	{
		m_MouseButtonBindings[event.Button] = event.Command;
		LOG_DEBUG("Input: Bound mouse button %i to %s", event.Button, event.Command.c_str());
	}

	return true;
}

void Systems::InputSystem::PublishCommand(int playerID, std::string command, bool release /*= false*/)
{
	if (release && command.at(0) == '+')
	{
		command[0] = '-';
	}

	Events::InputCommand e;
	e.PlayerID = playerID;
	e.Command = command;
	EventBroker->Publish(e);

	LOG_DEBUG("Input: Published command %s for player %i", e.Command.c_str(), playerID);
}
