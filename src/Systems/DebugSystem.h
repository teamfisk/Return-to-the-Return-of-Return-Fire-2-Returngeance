#ifndef DebugSystem_h__
#define DebugSystem_h__

#include "System.h"
#include "Components/Transform.h"
#include "Events/KeyDown.h"
#include "Events/PlaySound.h"

namespace Systems
{

class DebugSystem : public System
{
public:
	DebugSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
		: System(world, eventBroker) { }

	void Initialize() override;

	void Update(double dt) override;

	EventRelay<Events::KeyDown> m_EKeyDown;
	bool OnKeyDown(const Events::KeyDown &event);
	
	//void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
};

}
#endif // DebugSystem_h__