#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();

	m_ResourceManager.Preload("Model", "Models/Placeholders/PhysicsTest/Plane.obj");
	m_ResourceManager.Preload("Model", "Models/Placeholders/PhysicsTest/ArrowCube.obj");

	RegisterComponents();

	{
		auto camera = CreateEntity();
		auto transform = AddComponent<Components::Transform>(camera, "Transform");
		transform->Position.z = 20.f;
		transform->Position.y = 20.f;
		transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
		auto cameraComp = AddComponent<Components::Camera>(camera, "Camera");
		cameraComp->FarClip = 2000.f;
		AddComponent(camera, "Input");
		auto freeSteering = AddComponent<Components::FreeSteering>(camera, "FreeSteering");
	}


	{
		auto ground = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground, "Transform");
		transform->Position = glm::vec3(0, 0, 0);
		transform->Scale = glm::vec3(1000.0f, 1.0f, 1000.0f);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ground, "Model");
		model->ModelFile = "Models/Placeholders/PhysicsTest/Cube.obj";
		auto box = AddComponent<Components::Box>(ground, "Box");
		box->Width = 500;
		box->Height = 0.5;
		box->Depth = 500;

		auto physics = AddComponent<Components::Physics>(ground, "Physics");
		physics->Mass = 10;
	}

	/*{
		auto TankTest = CreateEntity();
		auto transform = AddComponent<Components::Transform>(TankTest, "Transform");
		transform->Position = glm::vec3(1.5f, 0.7f, 5.f);

		auto model = AddComponent<Components::Model>(TankTest, "Model");
		model->ModelFile = "Models/Placeholders/tank/Chassi.obj";
	}*/

	/*for(int i = 0; i < 2; i++)
	{
		auto light = CreateEntity();
		auto transform = AddComponent<Components::Transform>(light, "Transform");
		transform->Position = glm::vec3((float)(2*i)*glm::sin((float)i), 3, (float)(2*i)*glm::cos((float)i));

		auto pointLight = AddComponent<Components::PointLight>(light, "PointLight");
		pointLight->Specular = glm::vec3(0.1f, 0.1f, 0.1f);
		pointLight->Diffuse = glm::vec3(0.05f, 0.36f, 1.f);
		pointLight->constantAttenuation = 0.03f;
		pointLight->linearAttenuation = 0.009f;
		pointLight->quadraticAttenuation = 0.07f;
		pointLight->spotExponent = 0.0f;

		auto model = AddComponent<Components::Model>(light, "Model");
		model->ModelFile = "Models/Placeholders/PhysicsTest/PointLight.obj";
	}*/

	/*for(int i = 0; i < 3; i++)
	{
		auto ball = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ball, "Transform");
		transform->Position = glm::vec3(i/5.f, 5 + i*2, i/5.f);
		transform->Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ball, "Model");
		model->ModelFile = "Models/Placeholders/PhysicsTest/Sphere.obj";
		auto sphere = AddComponent<Components::Sphere>(ball, "Sphere");
		sphere->Radius = 0.05;
		auto physics = AddComponent<Components::Physics>(ball, "Physics");
		physics->Mass = 1;
	}*/

	/*{
		auto entity = CreateEntity();
		AddComponent(entity, "Transform");
		auto emitter = AddComponent<Components::SoundEmitter>(entity, "SoundEmitter");
		emitter->Path = "Sounds/korvring.wav";
		emitter->Loop = true;
		GetSystem<Systems::SoundSystem>("SoundSystem")->PlaySound(emitter);
	}*/

	{
		for(int i = 0; i < 1 ; i++)
		{
			// Particle emitter
			auto ent = CreateEntity();
			auto transform = AddComponent<Components::Transform>(ent, "Transform");
			transform->Orientation = glm::angleAxis(glm::pi<float>()/2, glm::vec3(1,0,0));
			transform->Position = glm::vec3(i * 10, 20, 0);
			auto emitter = AddComponent<Components::ParticleEmitter>(ent, "ParticleEmitter");
			emitter->LifeTime = 4;
			emitter->SpawnCount = 4;
			emitter->SpreadAngle = glm::pi<float>()/4;
			emitter->SpawnFrequency = 0.08;
			auto model = AddComponent<Components::Model>(ent, "Model");
			model->ModelFile = "Models/Placeholders/PhysicsTest/PointLight.obj";
			
			auto particleEnt = CreateEntity();
			AddComponent<Components::Transform>(particleEnt, "Transform");
			model = AddComponent<Components::Model>(particleEnt, "Model");
			model->ModelFile = "Models/Placeholders/PhysicsTest/PointLight.obj";

			emitter->ParticleTemplate = particleEnt;
		}
	}
}

void GameWorld::Update(double dt)
{
	World::Update(dt);
}

void GameWorld::RegisterComponents()
{
	m_ComponentFactory.Register("Transform", []() { return new Components::Transform(); }); 
	m_ComponentFactory.Register("Template", []() { return new Components::Template(); });	
	m_ComponentFactory.Register("Sphere", []() { return new Components::Sphere(); });
	m_ComponentFactory.Register("Box", []() { return new Components::Box (); });
}

void GameWorld::RegisterSystems()
{
	m_SystemFactory.Register("TransformSystem", [this]() { return new Systems::TransformSystem(this); });
	//m_SystemFactory.Register("LevelGenerationSystem", [this]() { return new Systems::LevelGenerationSystem(this); });
	m_SystemFactory.Register("InputSystem", [this]() { return new Systems::InputSystem(this, m_Renderer); });
	//m_SystemFactory.Register("CollisionSystem", [this]() { return new Systems::CollisionSystem(this); });
	m_SystemFactory.Register("ParticleSystem", [this]() { return new Systems::ParticleSystem(this); });
	//m_SystemFactory.Register("PlayerSystem", [this]() { return new Systems::PlayerSystem(this); });
	m_SystemFactory.Register("FreeSteeringSystem", [this]() { return new Systems::FreeSteeringSystem(this); });
	m_SystemFactory.Register("SoundSystem", [this]() { return new Systems::SoundSystem(this); });
	m_SystemFactory.Register("PhysicsSystem", [this]() { return new Systems::PhysicsSystem(this); });
	m_SystemFactory.Register("RenderSystem", [this]() { return new Systems::RenderSystem(this, m_Renderer); });
}

void GameWorld::AddSystems()
{
	AddSystem("TransformSystem");
	//AddSystem("LevelGenerationSystem");
	AddSystem("InputSystem");
	//AddSystem("CollisionSystem");
	AddSystem("ParticleSystem");
	//AddSystem("PlayerSystem");
	AddSystem("FreeSteeringSystem");
	AddSystem("SoundSystem");
	AddSystem("PhysicsSystem");
	AddSystem("RenderSystem");
}