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
	void Initialize() override;

	void Update(double dt) override;

private:
	std::shared_ptr<Renderer> m_Renderer;

	// Events
	EventRelay<Events::KeyDown> m_EKeyDown;
	bool OnKeyDown(const Events::KeyDown &event);
};

}

#endif // InputSystem_h__
