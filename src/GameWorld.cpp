#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();

	{
		auto camera = CreateEntity();
		auto transform = AddComponent<Components::Transform>(camera, "Transform");
		transform->Position.z = 150.f;
		transform->Position.y = 100.f;
		transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
		auto cameraComp = AddComponent<Components::Camera>(camera, "Camera");
		cameraComp->FarClip = 2000.f;
		AddComponent(camera, "Input");
		auto freeSteering = AddComponent<Components::FreeSteering>(camera, "FreeSteering");
	}

	{
		auto terrain = CreateEntity();
		auto transform = AddComponent<Components::Transform>(terrain, "Transform");
		transform->Scale = glm::vec3(1000.0f);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(terrain, "Model");
		model->ModelFile = "Models/PhysicsTest/Plane.obj";
		auto physics = AddComponent<Components::Physics>(terrain, "Physics");
		auto Box = AddComponent<Components::BoxShape>(terrain, "BoxShape");
		Box->Width = 1;
		Box->Height = 0;
		Box->Depth = 1;
		physics->Friction = 0.5f;
	}

	

	{
		auto entity = CreateEntity();
		auto transform = AddComponent<Components::Transform>(entity, "Transform");
		transform->Scale = glm::vec3(1.0f);
		transform->Position = glm::vec3(0, 15, 0);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		
		auto physics = AddComponent<Components::Physics>(entity, "Physics");
		physics->Mass = 10;
		physics->Friction = 0.5;

		auto compound = AddComponent<Components::CompoundShape>(entity, "CompoundShape");

		{
			auto entity0 = CreateEntity(entity);
			auto transform = AddComponent<Components::Transform>(entity0, "Transform");
			transform->Scale = glm::vec3(1.f);
			auto model = AddComponent<Components::Model>(entity0, "Model");
			model->ModelFile = "Models/PhysicsTest/Sphere.obj";

			auto sphere = AddComponent<Components::SphereShape>(entity0, "SphereShape");
			sphere->Radius = 0.5f;
		}

		{
			auto entity1 = CreateEntity(entity);
			auto transform = AddComponent<Components::Transform>(entity1, "Transform");
			transform->Scale = glm::vec3(1.0f);
			transform->Position = glm::vec3(-1, 1, 0);
			transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
			auto model = AddComponent<Components::Model>(entity1, "Model");
			model->ModelFile = "Models/PhysicsTest/Cube.obj";

			auto box = AddComponent<Components::BoxShape>(entity1, "BoxShape");
			box->Width = 0.5;
			box->Height = 0.5;
			box->Depth = 0.5;
		}


		{
			auto entity = CreateEntity();
			auto transform = AddComponent<Components::Transform>(entity, "Transform");
			transform->Scale = glm::vec3(1.0f);
			transform->Position = glm::vec3(1, 6, 0);
			transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
			auto model = AddComponent<Components::Model>(entity, "Model");
			model->ModelFile = "Models/PhysicsTest/Cube.obj";

			auto physics = AddComponent<Components::Physics>(entity, "Physics");
			physics->Mass = 10;

			auto box = AddComponent<Components::BoxShape>(entity, "BoxShape");
			box->Width = 0.5;
			box->Height = 0.5;
			box->Depth = 0.5;
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

	m_ComponentFactory.Register("Physics", []() { return new Components::Physics(); });
	m_ComponentFactory.Register("CompoundShape", []() { return new Components::CompoundShape(); });
	m_ComponentFactory.Register("SphereShape", []() { return new Components::SphereShape(); });
	m_ComponentFactory.Register("BoxShape", []() { return new Components::BoxShape(); });

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