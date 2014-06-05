#include "PrecompiledHeader.h"
#include "FlagSystem.h"
#include "World.h"
void Systems::FlagSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Flag>([]() { return new Components::Flag(); });
}

void Systems::FlagSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_EEnterTrigger, &Systems::FlagSystem::OnEnterTrigger);
}

void Systems::FlagSystem::Update( double dt )
{

}

void Systems::FlagSystem::UpdateEntity( double dt, EntityID entity, EntityID parent )
{

}

bool Systems::FlagSystem::OnEnterTrigger( const Events::EnterTrigger &event )
{
	auto JeepSteeringComponent = m_World->GetComponent<Components::JeepSteering>(event.Entity);
	if(!JeepSteeringComponent)
		return false;

	auto teamComponent = m_World->GetComponent<Components::Team>(event.Trigger);
	auto playerComponent = m_World->GetComponent<Components::Player>(event.Entity);

	if(!teamComponent || !playerComponent)
		return false;
	if (teamComponent->TeamID == playerComponent->ID)
		return false;



	auto flagComponent = m_World->GetComponent<Components::Flag>(event.Trigger);
	if(!flagComponent)
		return false;

	PickUpFlag(event.Entity, event.Trigger);
	
	return true;
}
void Systems::FlagSystem::PickUpFlag( EntityID entity, EntityID trigger )
{
	m_World->RemoveComponent<Components::Trigger>(trigger);
	m_World->SetEntityParent(trigger, entity);

	auto triggerTransform = m_World->GetComponent<Components::Transform>(trigger);
	triggerTransform->Position = glm::vec3(-1.5f, 0.1f, 2.f);

	auto teamComponent = m_World->GetComponent<Components::Team>(trigger);
	auto playerComponent = m_World->GetComponent<Components::Player>(entity);

	Events::FlagPickup ePickup;
	ePickup.Player = playerComponent->ID;
	EventBroker->Publish(ePickup);
}

