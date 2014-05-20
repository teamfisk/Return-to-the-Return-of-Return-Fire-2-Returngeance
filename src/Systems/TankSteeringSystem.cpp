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

	auto playerComponent = m_World->GetComponent<Components::Player>(tankSteeringComponent->Player);
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
			eSetVelocity.Velocity = absoluteTransform.Orientation * (glm::vec3(0.f, 0.f, -1.f) * barrelSteeringComponent->ShotSpeed);
			EventBroker->Publish(eSetVelocity);
			m_TimeSinceLastShot[tankSteeringComponent->Barrel] = 0;


			auto clonePhysicsComponent = m_World->GetComponent<Components::Physics>(clone);
			//1,670m/s
			//25kg
			Events::ApplyPointImpulse ePointImpulse ;
			ePointImpulse.Entity = entity;
			ePointImpulse.Position = absoluteTransform.Position;
			ePointImpulse.Impulse = glm::normalize(absoluteTransform.Orientation * glm::vec3(0, 0, 1))  * clonePhysicsComponent->Mass * 1670.f;
			EventBroker->Publish(ePointImpulse);
		}

		m_TimeSinceLastShot[tankSteeringComponent->Barrel] += dt;
	}
}

bool Systems::TankSteeringSystem::OnCollision( const Events::Collision &e )
{
	// TODO:
	auto modelComponent = m_World->GetComponent<Components::Model>(e.Entity1);

	if (modelComponent && modelComponent->ModelFile == "Models/Placeholders/rocket/Rocket.obj")
	{
		auto transform = m_World->GetComponent<Components::Transform>(e.Entity1);
		m_World->GetSystem<Systems::ParticleSystem>()->CreateExplosion(
			transform->Position,
			1,
			60,
			"Textures/Sprites/NewtonTreeDeleteASAPPlease.png",
			glm::angleAxis(glm::pi<float>()/2, glm::vec3(1,0,0)),
			40,
			glm::pi<float>(),
			0.5f
			);
		//m_World->RemoveEntity(e.Entity1);
	}

	//LOG_INFO("Ent1 %i Ent2 %i", e.Entity1, e.Entity2);

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
		m_Vertical = -0.4f;
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

	return true;
}



