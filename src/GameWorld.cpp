#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();

	std::shared_ptr<Components::Transform> transform;
	std::shared_ptr<Components::Model> model;

	auto camera = CreateEntity();
	transform = AddComponent<Components::Transform>(camera, "Transform");
	transform->Position.z = 150.f;
	transform->Position.y = 100.f;
	transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
	auto cameraComp = AddComponent<Components::Camera>(camera, "Camera");
	cameraComp->FarClip = 2000.f;
	AddComponent(camera, "Input");
	auto freeSteering = AddComponent<Components::FreeSteering>(camera, "FreeSteering");
	
	
	auto terrain = CreateEntity();
	transform = AddComponent<Components::Transform>(terrain, "Transform");
	transform->Scale = glm::vec3(1000.0f);
	transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, glm::pi<float>()/20.f));
 	model = AddComponent<Components::Model>(terrain, "Model");
 	model->ModelFile = "Models/PhysicsTest/Plane.obj";
	auto physics = AddComponent<Components::Physics>(terrain, "Physics");
	auto Box = AddComponent<Components::BoxShape>(terrain, "BoxShape");
	Box->Width = 1;
	Box->Height = 0;
	Box->Depth = 1;
	physics->Friction = 0.5f;

	terrain = CreateEntity();
	transform = AddComponent<Components::Transform>(terrain, "Transform");
	transform->Scale = glm::vec3(1000.0f);
	transform->Position = glm::vec3(-500, 0, 0);
	transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, -glm::pi<float>() / 20.f));
	model = AddComponent<Components::Model>(terrain, "Model");
	model->ModelFile = "Models/PhysicsTest/Plane.obj";
	physics = AddComponent<Components::Physics>(terrain, "Physics");
	Box = AddComponent<Components::BoxShape>(terrain, "BoxShape");
	Box->Width = 1;
	Box->Height = 0;
	Box->Depth = 1;
	physics->Friction = 0.5f;



	auto Boxed = CreateEntity();
	transform = AddComponent<Components::Transform>(Boxed, "Transform");
	transform->Position.y = 100.f;
	transform->Position.x = 4.f;
	transform->Scale = glm::vec3(5.0f);
	model = AddComponent<Components::Model>(Boxed, "Model");
	model->ModelFile = "Models/PhysicsTest/Cube.obj";
	physics = AddComponent<Components::Physics>(Boxed, "Physics");
	physics->Mass = 10;
	physics->Friction = 0.5f; 
	Box = AddComponent<Components::BoxShape>(Boxed, "BoxShape");
	Box->Width = 1;
	Box->Height = 1;
	Box->Depth = 1;

	for (int i = 0; i < 20; i++)
	{
		auto Sphered = CreateEntity();
		transform = AddComponent<Components::Transform>(Sphered, "Transform");
		transform->Position.y = 15.f;
		transform->Position.x = 4.f;
		transform->Scale = glm::vec3(1.0f);
		model = AddComponent<Components::Model>(Sphered, "Model");
		model->ModelFile = "Models/PhysicsTest/Sphere.obj";
		physics = AddComponent<Components::Physics>(Sphered, "Physics");
		physics->Mass = 100;
		physics->Friction = 0.5f;
		auto sphere = AddComponent<Components::SphereShape>(Sphered, "SphereShape");
		sphere->Radius = 1;
		sphere->RollingFriction = 0.5f;


		{
			auto tank_top = CreateEntity(Sphered);
			transform = AddComponent<Components::Transform>(tank_top, "Transform");
			transform->Scale = glm::vec3(1.0f/100.f);
			model = AddComponent<Components::Model>(tank_top, "Model");
			model->ModelFile = "Models/tank/tank_top.obj";
			{
				auto tank_barrel = CreateEntity(tank_top);
				transform = AddComponent<Components::Transform>(tank_barrel, "Transform");
				
				model = AddComponent<Components::Model>(tank_barrel, "Model");
				model->ModelFile = "Models/tank/tank_barrel.obj";
			}
		}
	}
	

	/*auto tank = CreateEntity();
	transform = AddComponent<Components::Transform>(tank, "Transform");
	transform->Position.y = 10.f;
	transform->Position.x = 4.f;
	transform->Scale = glm::vec3(.01f);
	model = AddComponent<Components::Model>(tank, "Model");
	model->ModelFile = "Models/tank/tank_base.obj";
	physics = AddComponent<Components::Physics>(tank, "Physics");
	physics->Mass = 100;
	physics->Friction = 0.5f;*/
	

	/*auto sphere = AddComponent<Components::SphereShape>(tank, "SphereShape");
	sphere->Radius = 10.*(1.f / .01f);
	sphere->RollingFriction = 0.5f;*/
/*
	Box = AddComponent<Components::BoxShape>(tank, "BoxShape");
	Box->Width = 100;
	Box->Height = 50;
	Box->Depth = 100;*/

/*
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
	}*/


	//auto light = CreateEntity();
	//transform = AddComponent<Components::Transform>(light, "Transform");
	//transform->Position.y = 10.f;
	//auto pointLight = AddComponent<Components::PointLight>(light, "PointLight");
	//pointLight->Diffuse = glm::vec3(0.1f, 0.1f, 1.0f);
	//pointLight->linearAttenuation = 1.0f;
	//pointLight->quadraticAttenuation = 1.0f;
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

	m_ComponentFactory.Register("SphereShape", []() { return new Components::SphereShape(); });
	m_ComponentFactory.Register("BoxShape", []() { return new Components::BoxShape(); });
	m_ComponentFactory.Register("Physics", []() { return new Components::Physics(); });

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
	
	m_SystemFactory.Register("PhysicsSystem", [this]() { return new Systems::PhysicsSystem(this); });

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

	AddSystem("PhysicsSystem");
	
	AddSystem("RenderSystem");

	
}