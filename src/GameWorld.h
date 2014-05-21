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

class GameWorld : public World
{
public:
	GameWorld(std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<Renderer> renderer)
		: World(eventBroker), m_Renderer(renderer) { }

	void Initialize();

	void RegisterSystems() override;
	void AddSystems() override;
	void RegisterComponents() override;

	void Update(double dt);

private:
	std::shared_ptr<Renderer> m_Renderer;

	void BindKey(int keyCode, std::string command, float value);
	void BindMouseButton(int button, std::string command, float value);
	void BindGamepadAxis(Gamepad::Axis axis, std::string command, float value);
	void BindGamepadButton(Gamepad::Button button, std::string command, float value);
};

#endif // GameWorld_h__
