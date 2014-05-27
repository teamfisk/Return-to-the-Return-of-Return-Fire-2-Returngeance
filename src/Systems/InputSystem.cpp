#include "PrecompiledHeader.h"
#include "InputSystem.h"
#include "World.h"

void Systems::InputSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::Input>([]() { return new Components::Input(); });
}

void Systems::InputSystem::Initialize()
{
	// Subscribe to events
	EVENT_SUBSCRIBE_MEMBER(m_EKeyDown, &Systems::InputSystem::OnKeyDown);
	EVENT_SUBSCRIBE_MEMBER(m_EKeyUp, &Systems::InputSystem::OnKeyUp);
	EVENT_SUBSCRIBE_MEMBER(m_EMousePress, &Systems::InputSystem::OnMousePress);
	EVENT_SUBSCRIBE_MEMBER(m_EMouseRelease, &Systems::InputSystem::OnMouseRelease);
	EVENT_SUBSCRIBE_MEMBER(m_EGamepadAxis, &Systems::InputSystem::OnGamepadAxis);
	EVENT_SUBSCRIBE_MEMBER(m_EGamepadButtonDown, &Systems::InputSystem::OnGamepadButtonDown);
	EVENT_SUBSCRIBE_MEMBER(m_EGamepadButtonUp, &Systems::InputSystem::OnGamepadButtonUp);
	EVENT_SUBSCRIBE_MEMBER(m_EBindKey, &Systems::InputSystem::OnBindKey);
	EVENT_SUBSCRIBE_MEMBER(m_EBindMouseButton, &Systems::InputSystem::OnBindMouseButton);
	EVENT_SUBSCRIBE_MEMBER(m_EBindGamepadAxis, &Systems::InputSystem::OnBindGamepadAxis);
	EVENT_SUBSCRIBE_MEMBER(m_EBindGamepadButton, &Systems::InputSystem::OnBindGamepadButton);
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
		std::string command;
		float value;
		std::tie(command, value) = bindingIt->second;
		m_CommandKeyboardValues[command][event.KeyCode] = value;
		PublishCommand(1, command, GetCommandTotalValue(command));
	}

	return true;
}

bool Systems::InputSystem::OnKeyUp(const Events::KeyUp &event)
{
	auto bindingIt = m_KeyBindings.find(event.KeyCode);
	if (bindingIt != m_KeyBindings.end())
	{
		std::string command;
		float value;
		std::tie(command, value) = bindingIt->second;
		m_CommandKeyboardValues[command][event.KeyCode] = 0;
		PublishCommand(1, command, GetCommandTotalValue(command));;
	}

	return true;
}

bool Systems::InputSystem::OnMousePress(const Events::MousePress &event)
{
	auto bindingIt = m_MouseButtonBindings.find(event.Button);
	if (bindingIt != m_MouseButtonBindings.end())
	{
		std::string command;
		float value;
		std::tie(command, value) = bindingIt->second;
		m_CommandMouseButtonValues[command][event.Button] = value;
		PublishCommand(1, command, GetCommandTotalValue(command));
	}

	return true;
}

bool Systems::InputSystem::OnMouseRelease(const Events::MouseRelease &event)
{
	auto bindingIt = m_MouseButtonBindings.find(event.Button);
	if (bindingIt != m_MouseButtonBindings.end())
	{
		std::string command;
		float value;
		std::tie(command, value) = bindingIt->second;
		m_CommandMouseButtonValues[command][event.Button] = 0;
		PublishCommand(1, command, GetCommandTotalValue(command));
	}

	return true;
}

bool Systems::InputSystem::OnGamepadAxis(const Events::GamepadAxis &event)
{
	auto bindingIt = m_GamepadAxisBindings.find(event.Axis);
	if (bindingIt != m_GamepadAxisBindings.end())
	{
		std::string command;
		float value;
		std::tie(command, value) = bindingIt->second;
		m_CommandGamepadAxisValues[command][event.Axis] = event.Value * value;
		PublishCommand(event.GamepadID + 1, command, GetCommandTotalValue(command));
	}

	return true;
}

bool Systems::InputSystem::OnGamepadButtonDown(const Events::GamepadButtonDown &event)
{
	auto bindingIt = m_GamepadButtonBindings.find(event.Button);
	if (bindingIt != m_GamepadButtonBindings.end())
	{
		std::string command;
		float value;
		std::tie(command, value) = bindingIt->second;
		m_CommandGamepadButtonValues[command][event.Button] = value;
		PublishCommand(event.GamepadID + 1, command, GetCommandTotalValue(command));
	}

	return true;
}

bool Systems::InputSystem::OnGamepadButtonUp(const Events::GamepadButtonUp &event)
{
	auto bindingIt = m_GamepadButtonBindings.find(event.Button);
	if (bindingIt != m_GamepadButtonBindings.end())
	{
		std::string command;
		float value;
		std::tie(command, value) = bindingIt->second;
		m_CommandGamepadButtonValues[command][event.Button] = 0;
		PublishCommand(event.GamepadID + 1, command, GetCommandTotalValue(command));
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
		m_KeyBindings[event.KeyCode] = std::make_tuple(event.Command, event.Value);
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
		m_MouseButtonBindings[event.Button] = std::make_tuple(event.Command, event.Value);
		LOG_DEBUG("Input: Bound mouse button %i to %s", event.Button, event.Command.c_str());
	}

	return true;
}

bool Systems::InputSystem::OnBindGamepadAxis(const Events::BindGamepadAxis &event)
{
	if (event.Command.empty())
	{
		m_GamepadAxisBindings.erase(event.Axis);
	}
	else
	{
		m_GamepadAxisBindings[event.Axis] = std::make_tuple(event.Command, event.Value);
		LOG_DEBUG("Input: Bound gamepad axis %i to %s", event.Axis, event.Command.c_str());
	}

	return true;
}

bool Systems::InputSystem::OnBindGamepadButton(const Events::BindGamepadButton &event)
{
	if (event.Command.empty())
	{
		m_GamepadButtonBindings.erase(event.Button);
	}
	else
	{
		m_GamepadButtonBindings[event.Button] = std::make_tuple(event.Command, event.Value);
		LOG_DEBUG("Input: Bound gamepad axis %i to %s", event.Button, event.Command.c_str());
	}

	return true;
}

float Systems::InputSystem::GetCommandTotalValue(std::string command)
{
	float value = 0.f;

	auto keyboardIt = m_CommandKeyboardValues.find(command);
	if (keyboardIt != m_CommandKeyboardValues.end())
	{
		for (auto &key : keyboardIt->second)
		{
			value += key.second;
		}
	}

	auto mouseButtonIt = m_CommandMouseButtonValues.find(command);
	if (mouseButtonIt != m_CommandMouseButtonValues.end())
	{
		for (auto &button : mouseButtonIt->second)
		{
			value += button.second;
		}
	}

	auto gamepadAxisIt = m_CommandGamepadAxisValues.find(command);
	if (gamepadAxisIt != m_CommandGamepadAxisValues.end())
	{
		for (auto &axis : gamepadAxisIt->second)
		{
			value += axis.second;
		}
	}

	auto gamepadButtonIt = m_CommandGamepadButtonValues.find(command);
	if (gamepadButtonIt != m_CommandGamepadButtonValues.end())
	{
		for (auto &button : gamepadButtonIt->second)
		{
			value += button.second;
		}
	}

	return std::max(-1.f, std::min(value, 1.f));
}

void Systems::InputSystem::PublishCommand(int playerID, std::string command, float value)
{
	Events::InputCommand e;
	e.PlayerID = playerID;
	e.Command = command;
	e.Value = value;
	EventBroker->Publish(e);

	LOG_DEBUG("Input: Published command %s=%f for player %i", e.Command.c_str(), e.Value, playerID);
}
