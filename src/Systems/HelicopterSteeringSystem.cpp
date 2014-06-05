#include "PrecompiledHeader.h"
#include "HelicopterSteeringSystem.h"
#include "World.h"

void Systems::HelicopterSteeringSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::HelicopterSteering>([]() { return new Components::HelicopterSteering(); });
}

void Systems::HelicopterSteeringSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_ESpawnVehicle, &Systems::HelicopterSteeringSystem::OnSpawnVehicle);

	m_InputController = std::unique_ptr<HelicopterSteeringInputController>(new HelicopterSteeringInputController(EventBroker));
}

void Systems::HelicopterSteeringSystem::Update(double dt)
{

}

void Systems::HelicopterSteeringSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transform = m_World->GetComponent<Components::Transform>(entity);
	if (!transform)
		return;

	auto helicopterComponent = m_World->GetComponent<Components::HelicopterSteering>(entity);
	if (helicopterComponent)
	{
		glm::vec3 controllerRotationEuler = m_InputController->Rotation * (float)dt;
		transform->Orientation *= glm::quat(controllerRotationEuler);

		Events::ApplyForce e;
		e.Entity = entity;
		e.DeltaTime = dt;
		e.Force = glm::normalize(transform->Orientation * glm::vec3(0, 1, 0)) * (m_InputController->Power * 3000.f  * 9.82f * 8.f);
		EventBroker->Publish(e);
	}
}

bool Systems::HelicopterSteeringSystem::OnSpawnVehicle(const Events::SpawnVehicle &event)
{
	if (event.VehicleType != "Helicopter")
		return false;

	auto spawnPointComponents = m_World->GetComponentsOfType<Components::SpawnPoint>();
	if (!spawnPointComponents)
	{
		LOG_ERROR("Found no spawn points!");
		return false;
	}

	for (auto &component : *spawnPointComponents)
	{
		auto spawnPoint = component->Entity;
		auto spawnPointComponent = std::dynamic_pointer_cast<Components::SpawnPoint>(component);
		auto teamComponent = m_World->GetComponent<Components::Team>(spawnPoint);

		if (!teamComponent)
		{
			LOG_ERROR("SpawnPoint has no TeamComponent");
			continue;
		}

		if (teamComponent->TeamID != event.PlayerID)
			continue;

		Components::Transform absoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(spawnPoint);

		// Create a tank
		EntityID tank = CreateHelicopter(event.PlayerID);
		auto tankTransform = m_World->GetComponent<Components::Transform>(tank);
		tankTransform->Position = absoluteTransform.Position + glm::vec3(0, 10, 0);
		tankTransform->Orientation = absoluteTransform.Orientation * glm::angleAxis(glm::pi<float>() / 2.f, glm::vec3(0, 1, 0)); 
		// Set the viewport correctly
		Events::SetViewportCamera e;
		e.CameraEntity = m_World->GetProperty<EntityID>(tank, "Camera");
		if (event.PlayerID == 1)
			e.ViewportFrame = "Viewport1";
		else if (event.PlayerID == 2)
			e.ViewportFrame = "Viewport2";
		EventBroker->Publish(e);
	}

	return true;
}

EntityID Systems::HelicopterSteeringSystem::CreateHelicopter(int playerID)
{
	auto heli = m_World->CreateEntity();
	{
		{
			auto transform = m_World->AddComponent<Components::Transform>(heli);
			transform->Position = glm::vec3(0.f, 3.f, 10.f);
			auto physics = m_World->AddComponent<Components::Physics>(heli);
			physics->Mass = 3000;
			physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
			auto heliComponent = m_World->AddComponent<Components::HelicopterSteering>(heli);

			auto player = m_World->AddComponent<Components::Player>(heli);
			player->ID = playerID;
			m_World->AddComponent<Components::Input>(heli);
			m_World->AddComponent<Components::Listener>(heli);
			auto health = m_World->AddComponent<Components::Health>(heli);
			health->Amount = 100.f;
		}

		auto model = m_World->CreateEntity(heli);
		{
			auto transform = m_World->AddComponent<Components::Transform>(model);
			transform->Orientation = glm::quat(glm::vec3(0, -glm::pi<float>() / 2.f, 0.f));
			auto modelComponent = m_World->AddComponent<Components::Model>(model);
			modelComponent->ModelFile = "Models/Heli/Chassi/Chassi.obj";
		}
		m_World->CommitEntity(model);

		auto shape = m_World->CreateEntity(heli);
		{
			auto transform = m_World->AddComponent<Components::Transform>(shape);
			auto box = m_World->AddComponent<Components::BoxShape>(shape);
			box->Width = 5.f;
			box->Height = 1.8f;
			box->Depth = 7.f;
		}
		m_World->CommitEntity(shape);


		auto camera = m_World->CreateEntity(heli);
		{
			auto transform = m_World->AddComponent<Components::Transform>(camera);
			transform->Position.z = 16.f;
			transform->Position.y = 4.f;
			transform->Orientation = glm::quat(glm::vec3(-glm::radians(5.f), 0.f, 0.f));
			auto cameraComp = m_World->AddComponent<Components::Camera>(camera);
			cameraComp->FarClip = 2000.f;


			/*auto follow = m_World->AddComponent<Components::Follow>(cameraTower);
			follow->Entity = barrel;
			follow->Distance = 15.f;
			follow->FollowAxis = glm::vec3(1, 1, 1);*/
			m_World->SetProperty(heli, "Camera", camera);
		}
		m_World->CommitEntity(camera);

	}
	m_World->CommitEntity(heli);

	return heli;
}

bool Systems::HelicopterSteeringSystem::HelicopterSteeringInputController::OnCommand(const Events::InputCommand &event)
{
	if (event.Command == "horizontal")
	{
		Rotation.z = -event.Value;
	}
	else if (event.Command == "vertical")
	{
		Rotation.x = -event.Value;
	}

	else if (event.Command == "normal")
	{
		Power = event.Value;
	}

	return true;
}

bool Systems::HelicopterSteeringSystem::HelicopterSteeringInputController::OnMouseMove(const Events::MouseMove &event)
{
	return true;
}

void Systems::HelicopterSteeringSystem::HelicopterSteeringInputController::Update(double dt)
{
	
}
