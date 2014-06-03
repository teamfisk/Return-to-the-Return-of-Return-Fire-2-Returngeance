#include "PrecompiledHeader.h"
#include "GarageSystem.h"
#include "World.h"

void Systems::GarageSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Garage>([]() { return new Components::Garage(); });
	cf->Register<Components::SpawnPoint>([]() { return new Components::SpawnPoint(); });
}

void Systems::GarageSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_EEnterTrigger, &Systems::GarageSystem::OnEnterTrigger);
	EVENT_SUBSCRIBE_MEMBER(m_ELeaveTrigger, &Systems::GarageSystem::OnLeaveTrigger);
	EVENT_SUBSCRIBE_MEMBER(m_ECommand, &Systems::GarageSystem::OnCommand);
}

void Systems::GarageSystem::Update(double dt)
{

}

void Systems::GarageSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{

}

bool Systems::GarageSystem::OnEnterTrigger(const Events::EnterTrigger &event)
{
	EntityID garage = m_World->GetEntityParent(event.Trigger);
	if (garage == 0)
		return false;
	auto garageComponent = m_World->GetComponent<Components::Garage>(garage);
	if (!garageComponent)
		return false;

	std::string name = m_World->GetProperty<std::string>(event.Trigger, "Name");

	if (name == "BoundsTrigger")
	{
		ToggleGarage(garageComponent);
	}

	m_EntitiesInTrigger[event.Trigger].insert(event.Entity);

	return true;
}


bool Systems::GarageSystem::OnLeaveTrigger(const Events::LeaveTrigger &event)
{
	EntityID garage = m_World->GetEntityParent(event.Trigger);
	if (garage == 0)
		return false;
	auto garageComponent = m_World->GetComponent<Components::Garage>(garage);
	if (!garageComponent)
		return false;

	std::string name = m_World->GetProperty<std::string>(event.Trigger, "Name");

	if (name == "BoundsTrigger")
	{
		ToggleGarage(garageComponent);
	}

	m_EntitiesInTrigger[event.Trigger].erase(event.Entity);

	return true;
}

bool Systems::GarageSystem::OnCommand(const Events::InputCommand &event)
{
	if (event.Command == "use" && event.Value > 0)
	{
		for (auto &pair : m_EntitiesInTrigger)
		{
			EntityID trigger = pair.first;
			auto entities = pair.second;

			std::string name = m_World->GetProperty<std::string>(trigger, "Name");
			if (name != "ElevatorShaftTrigger")
				continue;

			auto triggerBaseParent = m_World->GetEntityBaseParent(trigger);
			auto triggerPlayerComponent = m_World->GetComponent<Components::Player>(triggerBaseParent);

			for (EntityID entity : entities)
			{
				auto entityPlayerComponent = m_World->GetComponent<Components::Player>(triggerBaseParent);
				if (triggerPlayerComponent == entityPlayerComponent)
				{
					EntityID garage = m_World->GetEntityParent(trigger);
					auto garageComponent = m_World->GetComponent<Components::Garage>(garage);
					ToggleGarage(garageComponent);
				}
			}
		}
	}

	return true;
}

void Systems::GarageSystem::ToggleGarage(Components::Garage* garageComponent)
{
	// Elevator
	if (garageComponent->Elevator != 0)
	{
		auto move = m_World->GetComponent<Components::Move>(garageComponent->Elevator);
		Events::Move e;
		e.Entity = garageComponent->Elevator;
		e.GoalPosition = move->GoalPosition;
		e.Speed = move->Speed;
		e.Queue = true;
		EventBroker->Publish(e);
		std::swap(move->GoalPosition, move->StartPosition);
	}
	
	// Left door
	if (garageComponent->DoorLeft != 0)
	{
		auto rotate = m_World->GetComponent<Components::Rotate>(garageComponent->DoorLeft);
		Events::Rotate e;
		e.Entity = garageComponent->DoorLeft;
		e.GoalRotation = rotate->GoalRotation;
		e.Time = rotate->Time;
		e.Queue = true;
		EventBroker->Publish(e);
		std::swap(rotate->GoalRotation, rotate->StartRotation);
	}
	// Right door
	if (garageComponent->DoorRight != 0)
	{
		auto rotate = m_World->GetComponent<Components::Rotate>(garageComponent->DoorRight);
		Events::Rotate e;
		e.Entity = garageComponent->DoorRight;
		e.GoalRotation = rotate->GoalRotation;
		e.Time = rotate->Time;
		e.Queue = true;
		EventBroker->Publish(e);
		std::swap(rotate->GoalRotation, rotate->StartRotation);
	}
}
