#include <array>

#include "System.h"
#include "Components/Transform.h"
#include "Components/FreeSteering.h"
#include "InputController.h"

namespace Systems
{

class FreeSteeringSystem : public System
{
public:
	FreeSteeringSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
		: System(world, eventBroker) { }

	void RegisterComponents(ComponentFactory* cf) override;
	void Initialize() override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

private:
	class FreeSteeringInputController;

	std::unique_ptr<FreeSteeringInputController> m_InputController;
};

class FreeSteeringSystem::FreeSteeringInputController : InputController
{
public:
	FreeSteeringInputController(std::shared_ptr<::EventBroker> eventBroker)
		: InputController(eventBroker)
		, SpeedMultiplier(1.f)
		, OrientationActive(false) { }

	glm::vec3 Movement;
	glm::quat Orientation;
	float SpeedMultiplier;
	bool OrientationActive;

protected:
	virtual bool OnCommand(const Events::InputCommand &event);
	virtual bool OnMouseMove(const Events::MouseMove &event);
};

}