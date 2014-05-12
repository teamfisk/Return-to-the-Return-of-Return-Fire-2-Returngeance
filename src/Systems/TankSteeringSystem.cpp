#include "PrecompiledHeader.h"
#include "TankSteeringSystem.h"
#include "World.h"

void Systems::TankSteeringSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register("TankSteering", []() { return new Components::TankSteering(); });
	cf->Register("TowerSteering", []() { return new Components::TowerSteering(); });
	cf->Register("BarrelSteering", []() { return new Components::BarrelSteering(); });
}

void Systems::TankSteeringSystem::Initialize()
{
	m_TankInputController = std::unique_ptr<TankSteeringInputController>(new TankSteeringInputController(EventBroker));
	m_TowerInputController = std::unique_ptr<TowerSteeringInputController>(new TowerSteeringInputController(EventBroker));
}

void Systems::TankSteeringSystem::Update(double dt)
{
	m_TankInputController->Update(dt);
	m_TowerInputController->Update(dt);
}

void Systems::TankSteeringSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto tankSteeringComponent = m_World->GetComponent<Components::TankSteering>(entity, "TankSteering");
	if(tankSteeringComponent)
	{
		Events::TankSteer e;
		e.Entity = entity;
		e.PositionX = m_TankInputController->PositionX;
		e.PositionY = m_TankInputController->PositionY;
		e.Handbrake = m_TankInputController->Handbrake;
		EventBroker->Publish(e);
	}

	auto towerSteeringComponent = m_World->GetComponent<Components::TowerSteering>(entity, "TowerSteering");
	if(towerSteeringComponent)
	{
		auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
		glm::quat orientation =  glm::angleAxis(towerSteeringComponent->Velocity * m_TowerInputController->TowerDirection * (float)dt, towerSteeringComponent->Axis);
		transformComponent->Orientation *= orientation;
	}

	auto barrelSteeringComponent = m_World->GetComponent<Components::BarrelSteering>(entity, "BarrelSteering");
	if(barrelSteeringComponent)
	{
		auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
		glm::quat orientation =  glm::angleAxis(barrelSteeringComponent->Velocity * m_TowerInputController->BarrelDirection * (float)dt, barrelSteeringComponent->Axis);
		transformComponent->Orientation *= orientation;
	}
}

void Systems::TankSteeringSystem::TankSteeringInputController::Update( double dt )
{
	PositionX = m_Horizontal;
	PositionY = m_Vertical;
}

void Systems::TankSteeringSystem::TowerSteeringInputController::Update( double dt )
{
	TowerDirection = m_TowerDirection;
	BarrelDirection = m_BarrelDirection;
}

bool Systems::TankSteeringSystem::TankSteeringInputController::OnCommand(const Events::InputCommand &event)
{
	float val = event.Value;
	if (event.Command == "horizontal")
	{
		m_Horizontal = val;
	}
	else if (event.Command == "vertical")
	{
		m_Vertical = val;
	}
	
	else if (event.Command == "handbrake")
	{
		Handbrake = val;
	}

	return true;
}

bool Systems::TankSteeringSystem::TowerSteeringInputController::OnCommand( const Events::InputCommand &event )
{
	float val = event.Value;
	if(event.Command == "tower_rotation")
	{
		m_TowerDirection = val;
	}
	else if(event.Command == "barrel_rotation")
	{
		m_BarrelDirection = val;
	}
	return true;
}

bool Systems::TankSteeringSystem::TowerSteeringInputController::OnMouseMove( const Events::MouseMove &event )
{
	return false;
}

bool Systems::TankSteeringSystem::TankSteeringInputController::OnMouseMove( const Events::MouseMove &event )
{
	return false;
}




