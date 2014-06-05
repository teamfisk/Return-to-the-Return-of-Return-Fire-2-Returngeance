#include "System.h"
#include "Components/Transform.h"
#include "Components/HelicopterSteering.h"
#include "Events/SetVelocity.h"
#include "Events/ApplyForce.h"
#include "Events/SpawnVehicle.h"
#include "InputController.h"
#include "Events/TankSteer.h"
#include "Events/SetVelocity.h"
#include "Events/ApplyForce.h"
#include "Events/ApplyPointImpulse.h"
#include "Events/CreateExplosion.h"
#include "Events/Collision.h"
#include "Components/Transform.h"
#include "Components/TankSteering.h"
#include "Components/TowerSteering.h"
#include "Components/BarrelSteering.h"
#include "Components/Physics.h"
#include "Components/Vehicle.h"
#include "Components/Player.h"
#include "Components/Model.h"
#include "Components/Team.h"
#include "Systems/TransformSystem.h"
#include "Systems/ParticleSystem.h"
#include "InputController.h"

#include "Components/Health.h"
#include "Components/TankShell.h"
#include "Components/SphereShape.h"
#include "Components/Listener.h"
#include "Components/SoundEmitter.h"

#include "Events/EnableCollisions.h"
#include "Events/DisableCollisions.h"
#include "Components/Trigger.h"
#include "Components/TriggerExplosion.h"
#include "Components/FrameTimer.h"

#include "Events/PlaySFX.h"
#include "Events/PlayBGM.h"

#include "Events/Damage.h"
#include "Components/Vehicle.h"
#include "Components/Player.h"

#include "Events/SpawnVehicle.h"
#include "Components/SpawnPoint.h"

#include "Components/Wheel.h"
#include "Components/MeshShape.h"
#include "Components/Camera.h"
#include "Components/BoxShape.h"
#include "Components/WheelPair.h"
#include "Components/Input.h"

#include "Events/SetViewportCamera.h"
#include "Events/GameOver.h"

namespace Systems
{

class HelicopterSteeringSystem : public System
{
public:
	HelicopterSteeringSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: System(world, eventBroker, resourceManager)
	{ }

	void RegisterComponents(ComponentFactory* cf) override;
	void Initialize() override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

private:
	EventRelay<HelicopterSteeringSystem, Events::SpawnVehicle> m_ESpawnVehicle;
	bool OnSpawnVehicle(const Events::SpawnVehicle &e);
	EntityID CreateHelicopter(int PlayerID);
	class HelicopterSteeringInputController;
	std::unique_ptr<HelicopterSteeringInputController> m_InputController;

	std::map<EntityID, double> m_TimeSinceLastShot;
};

class HelicopterSteeringSystem::HelicopterSteeringInputController : InputController<HelicopterSteeringSystem>
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