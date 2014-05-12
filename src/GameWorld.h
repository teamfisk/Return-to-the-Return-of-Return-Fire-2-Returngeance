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

#include "Components/Physics.h"
#include "Components/Sphere.h"
#include "Components/Box.h"
#include "Components/Vehicle.h"
#include "Components/Wheel.h"

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

	void BindKey(int keyCode, std::string command);
	void BindMouseButton(int button, std::string command);
};

#endif // GameWorld_h__
