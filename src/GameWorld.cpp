#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();

	std::shared_ptr<Components::Transform> transform;
	std::shared_ptr<Components::Model> model;

	auto camera = CreateEntity();
	transform = AddComponent<Components::Transform>(camera, "Transform");
	transform->Position.z = 5.f;
	auto cameraComp = AddComponent<Components::Camera>(camera, "Camera");
	AddComponent(camera, "Input");
	auto freeSteering = AddComponent<Components::FreeSteering>(camera, "FreeSteering");
	
	/*auto terrain = CreateEntity();
	transform = AddComponent<Components::Transform>(terrain, "Transform");
	transform->Scale = glm::vec3(.0005f);
	model = AddComponent<Components::Model>(terrain, "Model");
	model->ModelFile = "Models/terrain/terrain.obj";*/

	auto tank = CreateEntity();
	transform = AddComponent<Components::Transform>(tank, "Transform");
	transform->Scale = glm::vec3(.01f);
	model = AddComponent<Components::Model>(tank, "Model");
	model->ModelFile = "Models/tank/tank_base.obj";
	{
		auto tank_top = CreateEntity(tank);
		transform = AddComponent<Components::Transform>(tank_top, "Transform");
		//transform->Scale = glm::vec3(.0005f);
		model = AddComponent<Components::Model>(tank_top, "Model");
		model->ModelFile = "Models/tank/tank_top.obj";
		{
			auto tank_barrel = CreateEntity(tank_top);
			transform = AddComponent<Components::Transform>(tank_barrel, "Transform");
			//transform->Scale = glm::vec3(.0005f);
			model = AddComponent<Components::Model>(tank_barrel, "Model");
			model->ModelFile = "Models/tank/tank_barrel.obj";
		}
	}
}

void GameWorld::Update(double dt)
{
	World::Update(dt);
}

void GameWorld::RegisterComponents()
{
	m_ComponentFactory.Register("Bounds", []() { return new Components::Bounds(); });
	m_ComponentFactory.Register("Camera", []() { return new Components::Camera(); });
	m_ComponentFactory.Register("Collision", []() { return new Components::Collision(); });
	m_ComponentFactory.Register("DirectionalLight", []() { return new Components::DirectionalLight(); });
	m_ComponentFactory.Register("Input", []() { return new Components::Input(); });
	m_ComponentFactory.Register("Model", []() { return new Components::Model(); });
	m_ComponentFactory.Register("ParticleEmitter", []() { return new Components::ParticleEmitter(); });
	m_ComponentFactory.Register("PointLight", []() { return new Components::PointLight(); });
	m_ComponentFactory.Register("SoundEmitter", []() { return new Components::SoundEmitter(); });
	m_ComponentFactory.Register("Sprite", []() { return new Components::Sprite(); });
	m_ComponentFactory.Register("Template", []() { return new Components::Template(); });
	m_ComponentFactory.Register("Transform", []() { return new Components::Transform(); });
	m_ComponentFactory.Register("FreeSteering", []() { return new Components::FreeSteering(); });
}

void GameWorld::RegisterSystems()
{
	m_SystemFactory.Register("TransformSystem", [this]() { return new Systems::TransformSystem(this); });
	//m_SystemFactory.Register("LevelGenerationSystem", [this]() { return new Systems::LevelGenerationSystem(this); });
	m_SystemFactory.Register("InputSystem", [this]() { return new Systems::InputSystem(this, m_Renderer); });
	//m_SystemFactory.Register("CollisionSystem", [this]() { return new Systems::CollisionSystem(this); });
	////m_SystemFactory.Register("ParticleSystem", [this]() { return new Systems::ParticleSystem(this); });
	//m_SystemFactory.Register("PlayerSystem", [this]() { return new Systems::PlayerSystem(this); });
	m_SystemFactory.Register("FreeSteeringSystem", [this]() { return new Systems::FreeSteeringSystem(this); });
	//m_SystemFactory.Register("SoundSystem", [this]() { return new Systems::SoundSystem(this); });
	m_SystemFactory.Register("RenderSystem", [this]() { return new Systems::RenderSystem(this, m_Renderer); });
}

void GameWorld::AddSystems()
{
	AddSystem("TransformSystem");
	//AddSystem("LevelGenerationSystem");
	AddSystem("InputSystem");
	//AddSystem("CollisionSystem");
	////AddSystem("ParticleSystem");
	//AddSystem("PlayerSystem");
	AddSystem("FreeSteeringSystem");
	//AddSystem("SoundSystem");
	AddSystem("RenderSystem");
}