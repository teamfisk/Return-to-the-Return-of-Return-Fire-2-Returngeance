#include "System.h"
#include "Components/Transform.h"
#include "Components/HelicopterSteering.h"
#include "Events/SetVelocity.h"
#include "Events/ApplyForce.h"
#include "InputController.h"

namespace Systems
{

class HelicopterSteeringSystem : public System
{
public:
	HelicopterSteeringSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
		: System(world, eventBroker) { }

	void RegisterComponents(ComponentFactory* cf) override;
	void Initialize() override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

private:
	class HelicopterSteeringInputController;
	std::unique_ptr<HelicopterSteeringInputController> m_InputController;

	std::map<EntityID, double> m_TimeSinceLastShot;
};

class HelicopterSteeringSystem::HelicopterSteeringInputController : InputController
{
public:
	HelicopterSteeringInputController(std::shared_ptr<::EventBroker> eventBroker)
		: InputController(eventBroker)
		, Power(0.f)
	{ }

	float Power;
	glm::vec3 Rotation;

	void Update(double dt);
	
protected:
	bool OnCommand(const Events::InputCommand &event) override;
	bool OnMouseMove(const Events::MouseMove &event) override;

	
};





}