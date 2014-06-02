#include "PrecompiledHeader.h"
#include "TankSteeringSystem.h"
#include "World.h"

void Systems::TankSteeringSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::TankSteering>([]() { return new Components::TankSteering(); });
	cf->Register<Components::TowerSteering>([]() { return new Components::TowerSteering(); });
	cf->Register<Components::BarrelSteering>([]() { return new Components::BarrelSteering(); });
}

void Systems::TankSteeringSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_ECollision, &Systems::TankSteeringSystem::OnCollision);

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
		auto absoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(tankSteeringComponent->Barrel);
		glm::quat orientation =  glm::angleAxis(barrelSteeringComponent->TurnSpeed * inputController->BarrelDirection * (float)dt, barrelSteeringComponent->Axis);
		transformComponent->Orientation *= orientation;

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


			auto clonePhysicsComponent = m_World->GetComponent<Components::Physics>(clone);
			//1,670m/s
			//25kg
			Events::ApplyPointImpulse ePointImpulse ;
			ePointImpulse.Entity = entity;
			ePointImpulse.Position = absoluteTransform.Position;
			ePointImpulse.Impulse = glm::normalize(absoluteTransform.Orientation * glm::vec3(0, 0, 1))  * clonePhysicsComponent->Mass * 6.f * 1670.f;
			EventBroker->Publish(ePointImpulse);
		}

		m_TimeSinceLastShot[tankSteeringComponent->Barrel] += dt;
	}
}

bool Systems::TankSteeringSystem::OnCollision( const Events::Collision &e )
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
		for (auto &physComponent : *physicsComponents)
		{
			EntityID physicsEntity = std::dynamic_pointer_cast<Components::Physics>(physComponent)->Entity;
			auto physEntityTransform = m_World->GetComponent<Components::Transform>(physicsEntity);

			float distance = glm::distance(physEntityTransform->Position, shellTransform->Position);
			if (distance <= shellComponent->ExplosionRadius)
			{
				// DO STUFF! :D
				float radius = shellComponent->ExplosionRadius;
				float strength = (1.f - pow(distance / radius, 2)) * shellComponent->ExplosionStrength;
				glm::vec3 direction = glm::normalize(physEntityTransform->Position - shellTransform->Position);

				Events::ApplyPointImpulse e;
				e.Entity = physicsEntity;
				e.Impulse = direction * strength;
				e.Position = physEntityTransform->Position;
				EventBroker->Publish(e);

				auto health = m_World->GetComponent<Components::Health>(physicsEntity);
				if(health)
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



