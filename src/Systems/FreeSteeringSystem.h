#include <array>

#include "System.h"
#include "Components/Transform.h"
#include "Components/Input.h"
#include "Components/FreeSteering.h"

namespace Systems
{
class FreeSteeringSystem : public System
{
public:
	FreeSteeringSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
		: System(world, eventBroker) { }

	void RegisterComponents(ComponentFactory* cf) override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
};
}