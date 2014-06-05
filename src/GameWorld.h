#ifndef GameWorld_h__
#define GameWorld_h__

#include "World.h"
#include "Renderer.h"

#include "Systems/TransformSystem.h"
//#include "Systems/CollisionSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/DebugSystem.h"
//#include "Systems/LevelGenerationSystem.h"
#include "Systems/ParticleSystem.h"
//#include "Systems/PlayerSystem.h"
#include "Systems/FreeSteeringSystem.h"
#include "Systems/TankSteeringSystem.h"
#include "Systems/HelicopterSteeringSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/SoundSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/TriggerSystem.h"
#include "Systems/TimerSystem.h"
#include "Systems/DamageSystem.h"
#include "Systems/WheelPairSystem.h"
#include "Systems/FollowSystem.h"
#include "Systems/WallSystem.h"
#include "Systems/TowerSystem.h"
#include "Systems/GarageSystem.h"

#include "Components/Camera.h"
#include "Components/DirectionalLight.h"
#include "Components/Input.h"
#include "Components/Model.h"
#include "Components/ParticleEmitter.h"
#include "Components/Particle.h"
#include "Components/PointLight.h"
#include "Components/SoundEmitter.h"
#include "Components/Sprite.h"
#include "Components/Template.h"
#include "Components/Transform.h"
#include "Components/Viewport.h"

#include "Components/Physics.h"
#include "Components/SphereShape.h"
#include "Components/BoxShape.h"
#include "Components/Vehicle.h"
#include "Components/Wheel.h"
#include "Components/HingeConstraint.h"
#include "Components/TankSteering.h"
#include "Components/TowerSteering.h"
#include "Components/BarrelSteering.h"
#include "Components/Player.h"
#include "Components/Health.h"
#include "Components/Trigger.h"
#include "Components/Flag.h"
#include "Components/WheelPair.h"
#include "Components/Follow.h"
#include "Components/TriggerRotate.h"
#include "Components/Move.h"
#include "Components/Rotate.h"

class GameWorld : public World
{
public:
	GameWorld(std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: World(eventBroker, resourceManager)
	{ }

	void Initialize();

	void RegisterSystems() override;
	void AddSystems() override;
	void RegisterComponents() override;

	void Update(double dt);

private:
	void BindKey(int keyCode, std::string command, float value);
	void BindMouseButton(int button, std::string command, float value);
	void BindGamepadAxis(Gamepad::Axis axis, std::string command, float value);
	void BindGamepadButton(Gamepad::Button button, std::string command, float value);

	void CreateGate(EntityID parent, glm::vec3 position, glm::quat orientation);
	void AddTankWheelPair(EntityID tankEntity, glm::vec3 position, int axleID, bool steering);
	EntityID CreateJeep(int playerID);
	EntityID CreateWall(EntityID parent, glm::vec3 pos, glm::quat orientation);
	EntityID CreateTower(EntityID parent, glm::vec3 pos, int playerID);
	EntityID CreateGarage(EntityID parent, glm::vec3 Position, glm::quat orientation, int playerID);
	void CreateTerrain();
	void CreateBase(glm::quat orientation, int playerID);
	std::vector<EntityID> m_WallDebrisTemplates;
	EntityID m_ShotTemplate;
};

#endif // GameWorld_h__
