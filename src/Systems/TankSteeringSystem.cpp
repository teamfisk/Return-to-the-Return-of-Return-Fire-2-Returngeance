#include "PrecompiledHeader.h"
#include "TankSteeringSystem.h"
#include "World.h"

void Systems::TankSteeringSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register<Components::TankSteering>([]() { return new Components::TankSteering(); });
	cf->Register<Components::TowerSteering>([]() { return new Components::TowerSteering(); });
	cf->Register<Components::BarrelSteering>([]() { return new Components::BarrelSteering(); });
}

void Systems::TankSteeringSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_ECollision, &Systems::TankSteeringSystem::OnCollision);
	EVENT_SUBSCRIBE_MEMBER(m_ESpawnVehicle, &Systems::TankSteeringSystem::OnSpawnVehicle);

	for (int i = 0; i < 4; i++)
	{
		m_TankInputControllers[i] = std::shared_ptr<TankSteeringInputController>(new TankSteeringInputController(EventBroker, i + 1));
	}
}

void Systems::TankSteeringSystem::Update(double dt)
{
	for (int i = 0; i < 4; i++)
	{
		m_TankInputControllers[i]->Update(dt);
	}
}

void Systems::TankSteeringSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto tankSteeringComponent = m_World->GetComponent<Components::TankSteering>(entity);
	if(!tankSteeringComponent)
		return;

	auto playerComponent = m_World->GetComponent<Components::Player>(entity);
	if (!playerComponent)
		return;

	if (playerComponent->ID == 0)
		return;

	auto inputController = m_TankInputControllers[playerComponent->ID - 1];

	Events::TankSteer eSteering;
	eSteering.Entity = entity;
	eSteering.PositionX = inputController->PositionX;
	eSteering.PositionY = inputController->PositionY;
	eSteering.Handbrake = inputController->Handbrake;
	EventBroker->Publish(eSteering);

	auto towerSteeringComponent = m_World->GetComponent<Components::TowerSteering>(tankSteeringComponent->Turret);
	auto barrelSteeringComponent = m_World->GetComponent<Components::BarrelSteering>(tankSteeringComponent->Barrel);

	if(towerSteeringComponent)
	{
		auto transformComponent = m_World->GetComponent<Components::Transform>(tankSteeringComponent->Turret);
		glm::quat orientation =  glm::angleAxis(towerSteeringComponent->TurnSpeed * inputController->TowerDirection * (float)dt, towerSteeringComponent->Axis);
		transformComponent->Orientation *= orientation;
	}
	
	if(barrelSteeringComponent)
	{
		auto tankTransform = m_World->GetComponent<Components::Transform>(entity);
		auto transformComponent = m_World->GetComponent<Components::Transform>(tankSteeringComponent->Barrel);
		
		glm::quat orientation =  glm::angleAxis(barrelSteeringComponent->TurnSpeed * inputController->BarrelDirection * (float)dt, barrelSteeringComponent->Axis);
		transformComponent->Orientation *= orientation;

		glm::vec3 angles = glm::eulerAngles(transformComponent->Orientation);
		if(angles.x > barrelSteeringComponent->UpperRotationLimit)
		{
			angles.x  = barrelSteeringComponent->UpperRotationLimit;
			transformComponent->Orientation = glm::quat(angles);
		}
		else if(angles.x < barrelSteeringComponent->LowerRotationLimit)
		{
			angles.x  = barrelSteeringComponent->LowerRotationLimit;
			transformComponent->Orientation = glm::quat(angles);
		}

		auto absoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(tankSteeringComponent->Barrel);

		if(inputController->Shoot && m_TimeSinceLastShot[tankSteeringComponent->Barrel] > 0.5)
		{
			EntityID clone = m_World->CloneEntity(barrelSteeringComponent->ShotTemplate);
			auto templateAbsoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(barrelSteeringComponent->ShotTemplate);
			auto cloneTransform = m_World->GetComponent<Components::Transform>(clone);
			cloneTransform->Position = templateAbsoluteTransform.Position;
			cloneTransform->Orientation = absoluteTransform.Orientation * cloneTransform->Orientation;

			Events::SetVelocity eSetVelocity;
			eSetVelocity.Entity = clone;
			eSetVelocity.Velocity = tankTransform->Velocity + absoluteTransform.Orientation * (glm::vec3(0.f, 0.f, -1.f) * barrelSteeringComponent->ShotSpeed);
			EventBroker->Publish(eSetVelocity);
			m_TimeSinceLastShot[tankSteeringComponent->Barrel] = 0;

			{
				Events::CreateExplosion e;
				e.LifeTime = 1.5;
				e.ParticleScale.push_back(0.6);
				e.ParticleScale.push_back(1.8);
				e.ParticlesToSpawn = 1;
				e.Position = cloneTransform->Position;
				e.RelativeUpOrientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1,0,0));
				e.Speed = 1.7;
				e.SpreadAngle = glm::pi<float>()/100;
				e.spritePath = "Textures/Sprites/Smoke1.png";
				e.Emitting = true;
				e.SpawnFrequency = 0.2;
				e.Color = glm::vec4(0.6,0.6,0.6,1);
				EventBroker->Publish(e);
			}
			{
				Events::CreateExplosion e;
				e.LifeTime = 0.2;
				e.ParticleScale.push_back(1);
				//e.ParticleScale.push_back(2);
				e.ParticlesToSpawn = 1;
				e.Position = cloneTransform->Position;
				e.Speed = 0;
				e.SpreadAngle = glm::pi<float>();
				e.spritePath = "Textures/Sprites/MuzzleFlash.png";
				e.Color = glm::vec4(2, 2, 2, 0.2);
				EventBroker->Publish(e);
			}

			{
				Events::PlaySFX e;
				e.Resource = "Sounds/SFX/TankShot.mp3";
				e.Position = cloneTransform->Position;
				e.Loop = false;
				EventBroker->Publish(e);
			}

			auto clonePhysicsComponent = m_World->GetComponent<Components::Physics>(clone);
			//1,670m/s
			//25kg
			Events::ApplyPointImpulse ePointImpulse;
			ePointImpulse.Entity = entity;
			ePointImpulse.Position = absoluteTransform.Position;
			ePointImpulse.Impulse = glm::normalize(absoluteTransform.Orientation * glm::vec3(0, 0, 1))  * clonePhysicsComponent->Mass * 6.f * 1670.f;
			EventBroker->Publish(ePointImpulse);
		}

		m_TimeSinceLastShot[tankSteeringComponent->Barrel] += dt;
	}

// 	auto shot = m_World->GetComponent<Components::TankShell>(entity); 
// 	if(shot)
// 	{
// 		if(inputController->Shoot && m_TimeSinceLastShot[tankSteeringComponent->Barrel] > 0.5)
// 		{
// 			
// 		}
// 	}
}

bool Systems::TankSteeringSystem::OnCollision(const Events::Collision &e)
{
	if(m_World->ValidEntity(e.Entity1) && m_World->ValidEntity(e.Entity2))
	{
		auto tankShell1 = m_World->GetComponent<Components::TankShell>(e.Entity1);
		auto tankShell2 = m_World->GetComponent<Components::TankShell>(e.Entity2);

		EntityID shellEntity = 0;
		Components::TankShell* shellComponent;
		EntityID otherEntity = 0;

		if (tankShell1)
		{
			shellEntity = e.Entity1;
			otherEntity = e.Entity2;
			shellComponent = tankShell1;
		}
		else if (tankShell2)
		{
			shellEntity = e.Entity2;
			otherEntity = e.Entity1;
			shellComponent = tankShell2;
		}
		else
		{
			return false;
		}

		if(m_World->GetComponent<Components::Template>(shellEntity))
			return false;

		auto physicsComponents = m_World->GetComponentsOfType<Components::Physics>();
		auto shellTransform = m_World->GetComponent<Components::Transform>(shellEntity);
		//auto otherTransform = m_World->GetComponent<Components::Transform>(otherEntity);

		{
			Events::CreateExplosion e;
			e.LifeTime = 3;
			e.ParticleScale.push_back(8);
			e.ParticlesToSpawn = 20;
			e.Position = shellTransform->Position;
			e.RelativeUpOrientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1, 0, 0));
			e.Speed = 3;
			e.SpreadAngle = glm::pi<float>();
			e.spritePath = "Textures/Sprites/Smoke1.png";
			e.Color = glm::vec4(0.6, 0.6, 0.6, 1);
			EventBroker->Publish(e);
			e.LifeTime = 0.7;
			e.ParticleScale.push_back(12);
			e.ParticlesToSpawn = 10;
			e.Position = shellTransform->Position;
			e.RelativeUpOrientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1, 0, 0));
			e.Speed = 17;
			e.SpreadAngle = glm::pi<float>();
			e.spritePath = "Textures/Sprites/Fire.png";
			EventBroker->Publish(e);
			e.LifeTime = 0.2;
			e.ParticleScale.push_back(1);
			e.ParticleScale.push_back(15);
			e.ParticlesToSpawn = 5;
			e.Position = shellTransform->Position;
			e.RelativeUpOrientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1, 0, 0));
			e.Speed = 6;
			e.SpreadAngle = glm::pi<float>();
			e.spritePath = "Textures/Sprites/Blast1.png";
			e.Color = glm::vec4(2, 2, 2, 0.2);
			EventBroker->Publish(e);
			
			/*Events::CreateExplosion e;
			e.LifeTime = 1.5;
			e.ParticleScale.push_back(1);
			e.ParticleScale.push_back(6);
			e.ParticlesToSpawn = 20;
			e.Position = shellTransform->Position;
			e.SpreadAngle = glm::radians<float>(180) / 4;
			e.RelativeUpOrientation = glm::angleAxis(glm::radians<float>(90), glm::vec3(1,0,0));
			e.UseGoalVector = true;
			e.GoalVelocity = glm::vec3(0,-12,0);
			e.Speed = 12;
			e.spritePath = "Textures/Sprites/Splash.png";
			e.Color = glm::vec4(5.f,5.f,5.f,1);

			EventBroker->Publish(e);
			e.LifeTime = 1.5;
			e.ParticleScale.push_back(1);
			e.ParticleScale.push_back(6);
			e.ParticlesToSpawn = 20;
			e.Position = shellTransform->Position;
			e.SpreadAngle = glm::radians<float>(180)/ 4;
			e.RelativeUpOrientation = glm::angleAxis(glm::radians<float>(90), glm::vec3(1,0,0));
			e.UseGoalVector = true;
			e.GoalVelocity = glm::vec3(0,-12,0);
			e.Speed = 12;
			e.spritePath = "Textures/Sprites/Splash.png";
			e.Color = glm::vec4(1.f,1.f,1.f,1);
			EventBroker->Publish(e);*/
		}
		{
			Events::PlaySFX e;
			e.MinDistance = 150.f;
			e.Position = shellTransform->Position;
			e.Resource = "Sounds/SFX/Explosion.wav";
			EventBroker->Publish(e);
		}

		// Direct damage
		auto health = m_World->GetComponent<Components::Health>(otherEntity);
		if (health)
		{
			Events::Damage d;
			d.Entity = otherEntity;
			d.Amount = shellComponent->Damage;
			EventBroker->Publish(d);
		}

		// Splash damage
		for (auto &physComponent : *physicsComponents)
		{
			EntityID physicsEntity = physComponent->Entity;
			// No splash damage to the direct hit target
			if (physicsEntity == otherEntity)
				continue;
			auto physEntityTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(physicsEntity);

			float distance = glm::distance(physEntityTransform.Position, shellTransform->Position);
			if (distance <= shellComponent->ExplosionRadius)
			{
				// DO STUFF! :D
				float radius = shellComponent->ExplosionRadius;
				float strength = (1.f - pow(distance / radius, 2)) * shellComponent->ExplosionStrength;
				glm::vec3 direction = glm::normalize(physEntityTransform.Position - shellTransform->Position);

				Events::ApplyPointImpulse e;
				e.Entity = physicsEntity;
				e.Impulse = direction * strength;
				e.Position = physEntityTransform.Position;
				EventBroker->Publish(e);

				auto health = m_World->GetComponent<Components::Health>(physicsEntity);
				if (health && health->VulnerableToSplash)
				{
					Events::Damage d;
					d.Entity = physicsEntity;
					d.Amount =  (1.f - pow(distance / radius, 2)) * shellComponent->Damage;
					EventBroker->Publish(d);
				}
				
				m_World->RemoveEntity(shellEntity);
			}
		}

		//if(tankShell1)
		//{
		//	LOG_DEBUG("%i collided with %i", e.Entity1, e.Entity2);
		//	auto transform = m_World->GetComponent<Components::Transform>(e.Entity1);
		//	//m_World->GetSystem<Systems::ParticleSystem>()->CreateExplosion(transform->Position, 1, 60, "Textures/Sprites/NewtonTreeDeleteASAPPlease.png", glm::angleAxis(glm::pi<float>()/2, glm::vec3(1,0,0)), 40, glm::pi<float>(), 0.5f);

		//	glm::vec3 pointOfImpact = transform->Position;


		//	{
		//		auto ent = m_World->CreateEntity();
		//		LOG_DEBUG("Created trigger entity %i", ent);
		//		auto transform = m_World->AddComponent<Components::Transform>(ent);
		//		transform->Position = pointOfImpact;

		//		auto trigger = m_World->AddComponent<Components::Trigger>(ent);
		//		trigger->TriggerOnce = true;
		//		auto frameTimer = m_World->AddComponent<Components::FrameTimer>(ent);
		//		frameTimer->Frames = 100;
		//		auto explosion = m_World->AddComponent<Components::TriggerExplosion>(ent);
		//		explosion->MaxVelocity = 50.f;
		//		explosion->Radius = 30.f;
		//		{
		//			auto shape = m_World->CreateEntity(ent);
		//			auto transformshape = m_World->AddComponent<Components::Transform>(shape);
		//			auto sphere = m_World->AddComponent<Components::SphereShape>(shape);
		//			sphere->Radius = 30.f;
		//			m_World->CommitEntity(shape);

		//		}
		//		m_World->CommitEntity(ent);
		//	}
		//	
		//	m_World->RemoveEntity(e.Entity1);
		//}

		//if(tankShell2)
		//{
		//	LOG_DEBUG("%i collided with %i", e.Entity1, e.Entity2);
		//	auto transform = m_World->GetComponent<Components::Transform>(e.Entity2);
		//	//m_World->GetSystem<Systems::ParticleSystem>()->CreateExplosion(transform->Position, 1, 60, "Textures/Sprites/NewtonTreeDeleteASAPPlease.png", glm::angleAxis(glm::pi<float>()/2, glm::vec3(1,0,0)), 40, glm::pi<float>(), 0.5f);

		//	glm::vec3 pointOfImpact = transform->Position;


		//	{
		//		auto ent = m_World->CreateEntity();
		//		LOG_DEBUG("Created trigger entity %i", ent);
		//		auto transform = m_World->AddComponent<Components::Transform>(ent);
		//		transform->Position = pointOfImpact;

		//		auto trigger = m_World->AddComponent<Components::Trigger>(ent);
		//		trigger->TriggerOnce = true;
		//		auto frameTimer = m_World->AddComponent<Components::FrameTimer>(ent);
		//		frameTimer->Frames = 100;
		//		auto explosion = m_World->AddComponent<Components::TriggerExplosion>(ent);
		//		explosion->MaxVelocity = 50.f;
		//		explosion->Radius = 30.f;
		//		{
		//			auto shape = m_World->CreateEntity(ent);
		//			auto transformshape = m_World->AddComponent<Components::Transform>(shape);
		//			auto sphere = m_World->AddComponent<Components::SphereShape>(shape);
		//			sphere->Radius = 30.f;
		//			m_World->CommitEntity(shape);

		//		}
		//		m_World->CommitEntity(ent);
		//	}

		//	m_World->RemoveEntity(e.Entity2);
		//}
	}

	return true;
}

bool Systems::TankSteeringSystem::OnSpawnVehicle(const Events::SpawnVehicle &event)
{
	if (event.VehicleType != "Tank")
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
		auto teamComponent =  m_World->GetComponent<Components::Team>(spawnPoint);

		if(!teamComponent)
		{
			LOG_ERROR("SpawnPoint has no TeamComponent");
			continue;
		}

		if (teamComponent->TeamID != event.PlayerID)
			continue;
		
		Components::Transform absoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(spawnPoint);

		// Create a tank
		EntityID tank = CreateTank(event.PlayerID);
		auto tankTransform = m_World->GetComponent<Components::Transform>(tank);
 		tankTransform->Position = absoluteTransform.Position;
 		tankTransform->Orientation = absoluteTransform.Orientation;
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

EntityID Systems::TankSteeringSystem::CreateTank(int playerID)
{
	auto tank = m_World->CreateEntity();
	auto transform = m_World->AddComponent<Components::Transform>(tank);
	transform->Position = glm::vec3(0, 5, 0);
	//transform->Orientation = glm::angleAxis(0.f, glm::vec3(0, 1, 0));
	auto physics = m_World->AddComponent<Components::Physics>(tank);
	physics->Mass = 63000 - 16000;
	physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
	auto vehicle = m_World->AddComponent<Components::Vehicle>(tank);
	vehicle->MaxTorque = 8000.f;
	vehicle->MaxSteeringAngle = 90.f;
	vehicle->MaxSpeedFullSteeringAngle = 4.f;
	vehicle->MinRPM = 200.0f; 
	vehicle->OptimalRPM = 2500.0f,
	vehicle->MaxRPM = 4000.0f; 
	vehicle->TopSpeed = 90.0f;
	vehicle->SpringDamping = 1.f;
	vehicle->UpshiftRPM = 3500.0f;
	vehicle->DownshiftRPM = 1000.0f;
	vehicle->gearsRatio0 = 3.0f;
	vehicle->gearsRatio1 = 2.25f; 
	vehicle->gearsRatio2 = 1.5f;
	vehicle->gearsRatio3 = 1.0f;

	auto player = m_World->AddComponent<Components::Player>(tank);
	player->ID = playerID;
	auto tankSteering = m_World->AddComponent<Components::TankSteering>(tank);
	m_World->AddComponent<Components::Input>(tank);
	auto health = m_World->AddComponent<Components::Health>(tank);
	health->Amount = 100.f;
	m_World->AddComponent<Components::Listener>(tank);

	{
		auto shape = m_World->CreateEntity(tank);
		auto transform = m_World->AddComponent<Components::Transform>(shape);
		auto meshShape = m_World->AddComponent<Components::MeshShape>(shape);
		meshShape->ResourceName = "Models/Tank/TankCollisionShape.obj";
		m_World->CommitEntity(shape);
	}
	{
		auto shapeTower = m_World->CreateEntity(tank);
		auto transform = m_World->AddComponent<Components::Transform>(shapeTower);
		transform->Position = glm::vec3(0, 1.10633f, 1.03024f);
		auto box = m_World->AddComponent<Components::BoxShape>(shapeTower);
		box->Width = 1.298f;
		box->Height = 0.502f;
		box->Depth = 1.211f;
		m_World->CommitEntity(shapeTower);
	}

	{
		auto chassis = m_World->CreateEntity(tank);
		auto transform = m_World->AddComponent<Components::Transform>(chassis);
		transform->Position = glm::vec3(0, 0, 0);
		auto model = m_World->AddComponent<Components::Model>(chassis);
		model->ModelFile = "Models/Tank/tankBody.obj";
	}
	{
		auto tower = m_World->CreateEntity(tank);
		m_World->SetProperty(tower, "Name", "tower");
		auto transform = m_World->AddComponent<Components::Transform>(tower);
		transform->Position = glm::vec3(0.f, 0.68f, 0.9f);
		auto model = m_World->AddComponent<Components::Model>(tower);
		model->ModelFile = "Models/Tank/tankTop.obj";
		auto towerSteering = m_World->AddComponent<Components::TowerSteering>(tower);
		towerSteering->Axis = glm::vec3(0.f, 1.f, 0.f);
		towerSteering->TurnSpeed = glm::pi<float>() / 4.f;
		{
			auto barrel = m_World->CreateEntity(tower);
			auto transform = m_World->AddComponent<Components::Transform>(barrel);
			transform->Position = glm::vec3(-0.012f, 0.3f, -0.95);
			auto model = m_World->AddComponent<Components::Model>(barrel);
			model->ModelFile = "Models/Tank/tankBarrel.obj";
			auto barrelSteering = m_World->AddComponent<Components::BarrelSteering>(barrel);
			barrelSteering->Axis = glm::vec3(1.f, 0.f, 0.f);
			barrelSteering->TurnSpeed = glm::pi<float>() / 4.f;
			barrelSteering->ShotSpeed = 70.f;
			barrelSteering->LowerRotationLimit = glm::radians(-10.f);
			barrelSteering->UpperRotationLimit = glm::radians(40.f);
			{
				auto shot = m_World->CreateEntity(barrel);
				auto transform = m_World->AddComponent<Components::Transform>(shot);
				transform->Position = glm::vec3(0.35f, 0.f, -2.f);
				transform->Orientation = glm::angleAxis(-glm::pi<float>() / 2.f, glm::vec3(1, 0, 0));
				transform->Scale = glm::vec3(3.f);
				m_World->AddComponent<Components::Template>(shot);
				auto physics = m_World->AddComponent<Components::Physics>(shot);
				physics->Mass = 25.f;
				physics->MotionType = Components::Physics::MotionTypeEnum::Dynamic;
				physics->CollisionEvent = true;
				auto modelComponent = m_World->AddComponent<Components::Model>(shot);
				modelComponent->ModelFile = "Models/Placeholders/rocket/Rocket.obj";
				auto tankShellComponent = m_World->AddComponent<Components::TankShell>(shot);
				tankShellComponent->Damage = 20.f;
				tankShellComponent->ExplosionRadius = 20.f;
				tankShellComponent->ExplosionStrength = 140.f;
				{
					auto shape = m_World->CreateEntity(shot);
					auto transform = m_World->AddComponent<Components::Transform>(shape);
					auto boxShape = m_World->AddComponent<Components::BoxShape>(shape);
					boxShape->Width = 0.5f;
					boxShape->Height = 0.5f;
					boxShape->Depth = 0.5f;
					m_World->CommitEntity(shape);
				}
				m_World->CommitEntity(shot);
				barrelSteering->ShotTemplate = shot;


				auto cameraTower = m_World->CreateEntity(barrel);
				{
					auto transform = m_World->AddComponent<Components::Transform>(cameraTower);
					transform->Position.z = 16.f;
					transform->Position.y = 4.f;
					transform->Orientation = glm::quat(glm::vec3(-glm::radians(5.f), 0.f, 0.f));
					auto cameraComp = m_World->AddComponent<Components::Camera>(cameraTower);
					cameraComp->FarClip = 2000.f;
					/*auto follow = m_World->AddComponent<Components::Follow>(cameraTower);
					follow->Entity = barrel;
					follow->Distance = 15.f;
					follow->FollowAxis = glm::vec3(1, 1, 1);*/
				}

				m_World->SetProperty(tank, "Camera", cameraTower);
			}
			m_World->CommitEntity(barrel);
			tankSteering->Barrel = barrel;
		}
		m_World->CommitEntity(tower);
		tankSteering->Turret = tower;



	}



	//{
	//	auto lightentity = m_World->CreateEntity(tank);
	//	auto transform = m_World->AddComponent<Components::Transform>(lightentity);
	//	transform->Position = glm::vec3(0, 0, 0);
	//	auto light = m_World->AddComponent<Components::PointLight>(lightentity);
	//	//light->Diffuse = glm::vec3(128.f/255.f, 172.f/255.f, 242.f/255.f);
	//	//light->Specular = glm::vec3(1.f);
	//	/*light->ConstantAttenuation = 0.3f;
	//	light->LinearAttenuation = 0.003f;
	//	light->QuadraticAttenuation = 0.002f;*/
	//}

	// 		auto wheelpair = m_World->CreateEntity(tank);
	// 		SetProperty(wheelpair, "Name", "WheelPair");
	// 		AddComponent(wheelpair, "WheelPairThingy");

#pragma region Wheels
	//Create wheels
	float wheelOffset = -0.83f;
	const float suspensionStrength = 15.f;
	const float springLength = 0.3f;

	AddTankWheelPair(tank, glm::vec3(1.68f, wheelOffset, -1.715f), 0, true);
	AddTankWheelPair(tank, glm::vec3(-1.68f, wheelOffset, -1.715f), 0, true);
	AddTankWheelPair(tank, glm::vec3(1.68f, wheelOffset, 2.375), 1, false);
	AddTankWheelPair(tank, glm::vec3(-1.68f, wheelOffset, 2.375), 1, false);
#pragma endregion

	{
		auto entity = m_World->CreateEntity(tank);
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

	m_World->CommitEntity(tank);

	return tank;
}

void Systems::TankSteeringSystem::AddTankWheelPair(EntityID tankEntity, glm::vec3 position, int axleID, bool front)
{
	const float separation = 1.77f;
	const float suspensionStrength = 15.f;
	const float springLength = 0.3f;

	bool steering = front;

	auto wheelFront = m_World->CreateEntity(tankEntity);
	{
		auto transform = m_World->AddComponent<Components::Transform>(wheelFront);
		transform->Position = position - glm::vec3(0, 0, separation / 2.f);  // glm::vec3(1.68f, -0.83f - wheelOffset, -0.83f);
		//transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
#ifdef DEBUG
		auto model = m_World->AddComponent<Components::Model>(wheelFront);
		model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
#endif
		auto Wheel = m_World->AddComponent<Components::Wheel>(wheelFront);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = axleID;
		Wheel->Mass = 2000;
		Wheel->Radius = 0.6f;
		Wheel->Steering = steering;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 4.f;
		Wheel->ConnectedToHandbrake = true;
		Wheel->TorqueRatio = 0.125f;
		Wheel->Width = 0.6f;
		m_World->CommitEntity(wheelFront);
	}

	auto wheelBack = m_World->CreateEntity(tankEntity);
	{
		auto transform = m_World->AddComponent<Components::Transform>(wheelBack);
		transform->Position = position + glm::vec3(0, 0, separation / 2.f); // glm::vec3(1.68f, -0.83f - wheelOffset, -2.6f);
		//transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
#ifdef DEBUG
		auto model = m_World->AddComponent<Components::Model>(wheelBack);
		model->ModelFile = "Models/Tank/Fix/WheelPhysics.obj";
#endif
		auto Wheel = m_World->AddComponent<Components::Wheel>(wheelBack);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = axleID;
		Wheel->Mass = 2000;
		Wheel->Radius = 0.6f;
		Wheel->Steering = steering;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 4.f;
		Wheel->ConnectedToHandbrake = true;
		Wheel->TorqueRatio = 0.125f;
		Wheel->Width = 0.6f;
		m_World->CommitEntity(wheelBack);
	}

	auto wheelPair = m_World->CreateEntity(tankEntity);
	{
		{
			auto transform = m_World->AddComponent<Components::Transform>(wheelPair);
			transform->Position = position;
			//transform->Orientation = glm::quat(glm::vec3(0, glm::pi<float>() / 4.f, 0));

			auto pair = m_World->AddComponent<Components::WheelPair>(wheelPair);
			pair->FakeWheelFront = wheelFront;
			pair->FakeWheelBack = wheelBack;
		}

		auto modelEntity = m_World->CreateEntity(wheelPair);
		{
			auto transform = m_World->AddComponent<Components::Transform>(modelEntity);
			//transform->Position = glm::vec3(0.f, 0.35f, 0.f);
			if (front)
			{
				transform->Position = glm::vec3(0.f, 0.03f, 0.f);
				transform->Orientation = glm::quat(glm::vec3(0, glm::pi<float>(), 0));
			}
			else
			{
				transform->Position = glm::vec3(0.f, 0.17f, 0.f);
				transform->Scale = glm::vec3(1.f, 1.2f, 1.2f);
			}

			auto model = m_World->AddComponent<Components::Model>(modelEntity);
			model->ModelFile = "Models/Tank/tankWheel.obj";
		}
		m_World->CommitEntity(modelEntity);
	}
	m_World->CommitEntity(wheelPair);
}

void Systems::TankSteeringSystem::TankSteeringInputController::Update( double dt )
{
	PositionX = m_Horizontal;
	PositionY = m_Vertical;

	TowerDirection = m_TowerDirection;
	BarrelDirection = m_BarrelDirection;
	Shoot = m_Shoot;
}

bool Systems::TankSteeringSystem::TankSteeringInputController::OnCommand(const Events::InputCommand &event)
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

	// Turret
	if(event.Command == "tower_rotation")
	{
		m_TowerDirection = -val;
	}
	else if(event.Command == "barrel_rotation")
	{
		m_BarrelDirection = val;
	}

	else if (event.Command == "shoot")
	{
		m_Shoot = val > 0;
	}

	else if(event.Command == "EnableCollisions")
	{
		Events::EnableCollisions e;
		e.Layer1 = 1;
		e.Layer2 = 2;
		EventBroker->Publish(e);
	}
	else if(event.Command == "DisableCollisions")
	{
		Events::DisableCollisions e;
		e.Layer1 = 1;
		e.Layer2 = 2;
		EventBroker->Publish(e);
	}
	return true;
}

