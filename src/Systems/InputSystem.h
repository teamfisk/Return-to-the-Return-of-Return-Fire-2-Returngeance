#ifndef InputSystem_h__
#define InputSystem_h__

#include <array>

#include "System.h"
#include "Renderer.h"
#include "Components/Input.h"
#include "Events/KeyUp.h"
#include "Events/KeyDown.h"

namespace Systems
{

class InputSystem : public System
{
public:
	InputSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<Renderer> renderer)
		: System(world, eventBroker)
		, m_Renderer(renderer) { }

	void RegisterComponents(ComponentFactory* cf) override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

private:
	std::shared_ptr<Renderer> m_Renderer;
	static std::array<int, GLFW_KEY_LAST+1> m_CurrentKeyState;
	static std::array<int, GLFW_KEY_LAST+1> m_LastKeyState;
	std::array<int, GLFW_MOUSE_BUTTON_LAST+1> m_CurrentMouseState;
	std::array<int, GLFW_MOUSE_BUTTON_LAST+1> m_LastMouseState;
	float m_CurrentMouseDeltaX, m_CurrentMouseDeltaY;
	float m_LastMouseX, m_LastMouseY;

};

}

#endif // InputSystem_h__
