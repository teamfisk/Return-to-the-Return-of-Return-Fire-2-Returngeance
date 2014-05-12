#ifndef InputSystem_h__
#define InputSystem_h__

#include <array>
#include <unordered_map>
#include <boost/any.hpp>

#include "System.h"
#include "Components/Input.h"
#include "Events/KeyUp.h"
#include "Events/KeyDown.h"
#include "Events/MousePress.h"
#include "Events/MouseRelease.h"
#include "Events/BindKey.h"
#include "Events/BindMouseButton.h"
#include "Events/InputCommand.h"

namespace Systems
{

class InputSystem : public System
{
public:
	InputSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
		: System(world, eventBroker) { }

	void RegisterComponents(ComponentFactory* cf) override;
	void Initialize() override;

	void Update(double dt) override;

private:
	// Input binding tables
	std::unordered_map<int, std::tuple<std::string, float>> m_KeyBindings; // GLFW_KEY... -> command string & value
	std::unordered_map<std::string, float> m_KeyBindingValues; // command string -> command value
	std::unordered_map<int, std::string> m_MouseButtonBindings; // GLFW_MOUSE_BUTTON... -> command string

	// Input events
	EventRelay<Events::KeyDown> m_EKeyDown;
	bool OnKeyDown(const Events::KeyDown &event);
	EventRelay<Events::KeyUp> m_EKeyUp;
	bool OnKeyUp(const Events::KeyUp &event);
	EventRelay<Events::MousePress> m_EMousePress;
	bool OnMousePress(const Events::MousePress &event);
	EventRelay<Events::MouseRelease> m_EMouseRelease;
	bool OnMouseRelease(const Events::MouseRelease &event);
	// Input binding events
	EventRelay<Events::BindKey> m_EBindKey;
	bool OnBindKey(const Events::BindKey &event);
	EventRelay<Events::BindMouseButton> m_EBindMouseButton;
	bool OnBindMouseButton(const Events::BindMouseButton &event);

	void PublishCommand(int playerID, std::string command, float value);
};

}

#endif // InputSystem_h__
