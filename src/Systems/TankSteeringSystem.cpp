#include "PrecompiledHeader.h"
#include "TankSteeringSystem.h"
#include "World.h"

void Systems::TankSteeringSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register("TankSteering", []() { return new Components::TankSteering(); });
}

void Systems::TankSteeringSystem::Initialize()
{
	m_InputController = std::unique_ptr<TankSteeringInputController>(new TankSteeringInputController(EventBroker));
	m_InputController->PositionX = 0;
	m_InputController->PositionY = 0;
	m_InputController->Handbrake = false;
}

void Systems::TankSteeringSystem::Update(double dt)
{

}

void Systems::TankSteeringSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto tankSteeringComponent = m_World->GetComponent<Components::TankSteering>(entity, "TankSteering");
	if(tankSteeringComponent)
	{
		Events::TankSteer e;
		e.Entity = entity;
		e.PositionX = m_InputController->PositionX;
		e.PositionY = m_InputController->PositionY;
		e.Handbrake = m_InputController->Handbrake;
		EventBroker->Publish(e);
	}
}

bool Systems::TankSteeringSystem::TankSteeringInputController::OnCommand(const Events::InputCommand &event)
{
	float val = boost::any_cast<float>(event.Value);
	if (event.Command == "+right")
	{
		PositionX += val;
	}
	else if (event.Command == "-right")
	{
		PositionX -= val;
	}
	else if (event.Command == "+left")
	{
		PositionX += -val;
	}
	else if (event.Command == "-left")
	{
		PositionX -= -val;
	}
	else if (event.Command == "+forward")
	{
		PositionY += -val;
	}
	else if (event.Command == "-forward")
	{
		PositionY -= -val;
	}
	else if (event.Command == "+backward")
	{
		PositionY += val;
	}
	else if (event.Command == "-backward")
	{
		PositionY -= val;
	}

	else if (event.Command == "+handbrake")
	{
		Handbrake = true;
	}
	else if (event.Command == "-handbrake")
	{
		Handbrake = false;
	}
	return true;
}

bool Systems::TankSteeringSystem::TankSteeringInputController::OnMouseMove( const Events::MouseMove &event )
{
	return false;
}
