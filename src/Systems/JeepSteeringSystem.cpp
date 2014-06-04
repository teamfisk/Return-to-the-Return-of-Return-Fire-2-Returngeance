#include "PrecompiledHeader.h"
#include "JeepSteeringSystem.h"
#include "World.h"

void Systems::JeepSteeringSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::JeepSteering>([]() { return new Components::JeepSteering(); });
}

void Systems::JeepSteeringSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_ESpawnVehicle, &Systems::JeepSteeringSystem::OnSpawnVehicle);

	for (int i = 0; i < 4; i++)
	{
		m_JeepInputControllers[i] = std::shared_ptr<JeepSteeringInputController>(new JeepSteeringInputController(EventBroker, i + 1));
	}
}

void Systems::JeepSteeringSystem::Update(double dt)
{
	for (int i = 0; i < 4; i++)
	{
		m_JeepInputControllers[i]->Update(dt);
	}
}

void Systems::JeepSteeringSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto tankSteeringComponent = m_World->GetComponent<Components::JeepSteering>(entity);
	if(!tankSteeringComponent)
		return;

	auto playerComponent = m_World->GetComponent<Components::Player>(entity);
	if (!playerComponent)
		return;

	if (playerComponent->ID == 0)
		return;

	auto inputController = m_JeepInputControllers[playerComponent->ID - 1];


	Events::JeepSteer eSteering;
	eSteering.Entity = entity;
	eSteering.PositionX = inputController->PositionX;
	eSteering.PositionY = inputController->PositionY;
	eSteering.Handbrake = inputController->Handbrake;
	EventBroker->Publish(eSteering);
}

bool Systems::JeepSteeringSystem::OnSpawnVehicle(const Events::SpawnVehicle &event)
{
	
	if (event.VehicleType != "Jeep")
		return false;

	auto spawnPointComponents = m_World->GetComponentsOfType<Components::SpawnPoint>();
	if (!spawnPointComponents)
	{
		LOG_ERROR("Found no spawn points!");
		return false;
	}

	for (auto &spawnPointComponent : *spawnPointComponents)
	{
		auto spawnPoint = spawnPointComponent->Entity;
		auto spawnPointBaseParent = m_World->GetEntityBaseParent(spawnPoint);
		auto playerComponent = m_World->GetComponent<Components::Player>(spawnPointBaseParent);
		if (!playerComponent || playerComponent->ID != event.PlayerID)
			continue;
		
		Components::Transform absoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(spawnPoint);

		// Create a tank
		EntityID Jeep = CreateJeep(event.PlayerID);
		auto tankTransform = m_World->GetComponent<Components::Transform>(Jeep);
 		tankTransform->Position = glm::vec3(0, 50, 0);//absoluteTransform.Position;
// 		tankTransform->Orientation = absoluteTransform.Orientation;
		// Set the viewport correctly
		Events::SetViewportCamera e;
		e.CameraEntity = m_World->GetProperty<EntityID>(Jeep, "Camera");
		if (event.PlayerID == 1)
			e.ViewportFrame = "Viewport1";
		else if (event.PlayerID == 2)
			e.ViewportFrame = "Viewport2";
		EventBroker->Publish(e);
	}

	return true;
}

EntityID Systems::JeepSteeringSystem::CreateJeep(int playerID)
{
	auto jeep = m_World->CreateEntity();
	auto transform = m_World->AddComponent<Components::Transform>(jeep);
	transform->Position = glm::vec3(0, 5, 0);
	//transform->Orientation = glm::angleAxis(0.f, glm::vec3(0, 1, 0));
	auto physics = m_World->AddComponent<Components::Physics>(jeep);
	physics->Mass = 1600;
	physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
	auto vehicle = m_World->AddComponent<Components::Vehicle>(jeep);
	vehicle->MaxTorque = 400.f;
	vehicle->MaxSteeringAngle = 50.f;
	vehicle->MaxSpeedFullSteeringAngle = 20.f;
	vehicle->MinRPM = 1000.0f; 
	vehicle->OptimalRPM = 4000.0f,
	vehicle->MaxRPM = 6000.0f; 
	vehicle->TopSpeed = 90.0f;
	vehicle->SpringDamping = 1.f;
	vehicle->UpshiftRPM = 5500.0f;
	vehicle->DownshiftRPM = 1500.0f;
	vehicle->gearsRatio0 = 3.0f;
	vehicle->gearsRatio1 = 2.25f; 
	vehicle->gearsRatio2 = 1.5f;
	vehicle->gearsRatio3 = 1.0f;

	auto player = m_World->AddComponent<Components::Player>(jeep);
	player->ID = playerID;
	auto tankSteering = m_World->AddComponent<Components::JeepSteering>(jeep);
	m_World->AddComponent<Components::Input>(jeep);
	auto health = m_World->AddComponent<Components::Health>(jeep);
	health->Amount = 100.f;

	{
		auto shape = m_World->CreateEntity(jeep);
		auto transform = m_World->AddComponent<Components::Transform>(shape);
		auto meshShape = m_World->AddComponent<Components::MeshShape>(shape);
		meshShape->ResourceName = "Models/Jeep/Chassi/ChassiCollision.obj";
		m_World->CommitEntity(shape);
	}

	{
		auto chassis = m_World->CreateEntity(jeep);
		auto transform = m_World->AddComponent<Components::Transform>(chassis);
		transform->Position = glm::vec3(0, 0, 0);
		auto model = m_World->AddComponent<Components::Model>(chassis);
		model->ModelFile = "Models/Jeep/Chassi/Chassi.OBJ";
	}
	

	auto cameraTower = m_World->CreateEntity(jeep);
	{
		auto transform = m_World->AddComponent<Components::Transform>(cameraTower);
		transform->Position.z = 16.f;
		transform->Position.y = 4.f;
		transform->Orientation = glm::quat(glm::vec3(-glm::radians(5.f), 0.f, 0.f));
		auto cameraComp = m_World->AddComponent<Components::Camera>(cameraTower);
		cameraComp->FarClip = 2000.f;
		m_World->SetProperty(jeep, "Camera", cameraTower);
	}

	
			
		


#pragma region Wheels
	//Create wheels
	float wheelOffset = 0.f;//-0.83f;
	const float suspensionStrength = 15.f;
	const float springLength = 0.3f;

	AddJeepWheels(jeep);
#pragma endregion

	{
		auto entity = m_World->CreateEntity(jeep);
		auto transformComponent = m_World->AddComponent<Components::Transform>(entity);
		transformComponent->Position = glm::vec3(-2, -1.7, 2.0);
		transformComponent->Scale = glm::vec3(3, 3, 3);
		transformComponent->Orientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1, 0, 0));
		auto emitterComponent = m_World->AddComponent<Components::ParticleEmitter>(entity);
		emitterComponent->SpawnCount = 2;
		emitterComponent->SpawnFrequency = 0.005;
		emitterComponent->SpreadAngle = glm::pi<float>();
		emitterComponent->UseGoalVelocity = false;
		emitterComponent->LifeTime = 0.5;
		//emitterComponent->AngularVelocitySpectrum.push_back(glm::pi<float>() / 100);
		emitterComponent->ScaleSpectrum.push_back(glm::vec3(0.05));
		m_World->CommitEntity(entity);

		auto particleEntity = m_World->CreateEntity(entity);
		auto TEMP = m_World->AddComponent<Components::Transform>(particleEntity);
		TEMP->Scale = glm::vec3(0);
		auto spriteComponent = m_World->AddComponent<Components::Sprite>(particleEntity);
		spriteComponent->SpriteFile = "Models/Textures/Sprites/Dust.png";
		emitterComponent->ParticleTemplate = particleEntity;

		m_World->CommitEntity(particleEntity);
	}

	m_World->CommitEntity(jeep);

	return jeep;
}

void Systems::JeepSteeringSystem::AddJeepWheels(EntityID jeepEntity)
{
	//Create wheels
	float wheelOffset = 0.4f;
	float springLength = 0.3f;
	float suspensionStrength = 45.f;
	{
		auto wheel = m_World->CreateEntity(jeepEntity);
		auto transform = m_World->AddComponent<Components::Transform>(wheel);
		transform->Position = glm::vec3(1.9f, 0.5546f - wheelOffset, -0.9242f);
		transform->Scale = glm::vec3(1.0f);
		auto model = m_World->AddComponent<Components::Model>(wheel);
		model->ModelFile = "Models/Jeep/WheelFront/wheelFront.obj";
		auto Wheel = m_World->AddComponent<Components::Wheel>(wheel);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = 0;
		Wheel->Mass = 50;
		Wheel->Radius = 0.837f;
		Wheel->Steering = true;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 4.f;
		Wheel->ConnectedToHandbrake = true;
		m_World->CommitEntity(wheel);
	}

	{
		auto wheel = m_World->CreateEntity(jeepEntity);
		auto transform = m_World->AddComponent<Components::Transform>(wheel);
		transform->Position = glm::vec3(-1.9f, 0.5546f - wheelOffset, -0.9242f);
		transform->Scale = glm::vec3(1.0f);
		transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1));
		auto model = m_World->AddComponent<Components::Model>(wheel);
		model->ModelFile = "Models/Jeep/WheelFront/wheelFront.obj";
		auto Wheel = m_World->AddComponent<Components::Wheel>(wheel);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = 0;
		Wheel->Mass = 50;
		Wheel->Radius = 0.837f;
		Wheel->Steering = true;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 4.f;
		Wheel->ConnectedToHandbrake = true;
		m_World->CommitEntity(wheel);
	}

	{
		auto wheel = m_World->CreateEntity(jeepEntity);
		auto transform = m_World->AddComponent<Components::Transform>(wheel);
		transform->Position = glm::vec3(0.2726f, 0.2805f - wheelOffset, 1.9307f);
		auto model = m_World->AddComponent<Components::Model>(wheel);
		model->ModelFile = "Models/Jeep/WheelBack/wheelBack.obj";
		auto Wheel = m_World->AddComponent<Components::Wheel>(wheel);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = 1;
		Wheel->Mass = 50;
		Wheel->Radius = 0.737f;
		Wheel->Steering = false;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 4.f;
		Wheel->ConnectedToHandbrake = true;
		m_World->CommitEntity(wheel);
	}

	{
		auto wheel = m_World->CreateEntity(jeepEntity);
		auto transform = m_World->AddComponent<Components::Transform>(wheel);
		transform->Position = glm::vec3(-0.2726f, 0.2805f - wheelOffset, 1.9307f);
		transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1));
		auto model = m_World->AddComponent<Components::Model>(wheel);
		model->ModelFile = "Models/Jeep/WheelBack/wheelBack.obj";
		auto Wheel = m_World->AddComponent<Components::Wheel>(wheel);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = 1;
		Wheel->Mass = 50;
		Wheel->Radius = 0.737f;
		Wheel->Steering = false;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 4.f;
		Wheel->ConnectedToHandbrake = true;
		m_World->CommitEntity(wheel);
	}


}

void Systems::JeepSteeringSystem::JeepSteeringInputController::Update( double dt )
{
	PositionX = m_Horizontal;
	PositionY = m_Vertical;
}

bool Systems::JeepSteeringSystem::JeepSteeringInputController::OnCommand(const Events::InputCommand &event)
{
	if (event.PlayerID != this->PlayerID)
		return false;

	float val = event.Value;
	
	// Tank
	if (event.Command == "horizontal")
	{
		m_Horizontal = val;
	}
	else if (event.Command == "vertical")
	{
		m_Vertical = -val;
	}

	else if (event.Command == "handbrake")
	{
		Handbrake = val > 0;
	}
	return true;
}

