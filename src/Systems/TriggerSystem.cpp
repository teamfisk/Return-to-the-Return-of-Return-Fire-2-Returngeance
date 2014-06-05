#include "PrecompiledHeader.h"
#include "TriggerSystem.h"
#include "World.h"

void Systems::TriggerSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Trigger>([]() { return new Components::Trigger(); });
	cf->Register<Components::TriggerExplosion>([]() { return new Components::TriggerExplosion(); });
	cf->Register<Components::TriggerMove>([]() { return new Components::TriggerMove(); });
	cf->Register<Components::TriggerRotate>([]() { return new Components::TriggerRotate(); });
}

void Systems::TriggerSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_EEnterTrigger, &Systems::TriggerSystem::OnEnterTrigger);
	EVENT_SUBSCRIBE_MEMBER(m_ELeaveTrigger, &Systems::TriggerSystem::OnLeaveTrigger);
}

void Systems::TriggerSystem::Update( double dt )
{

}

void Systems::TriggerSystem::UpdateEntity( double dt, EntityID entity, EntityID parent )
{

}


void Systems::TriggerSystem::OnComponentRemoved(EntityID entity, std::string type, Component* component )
{

}

void Systems::TriggerSystem::OnEntityCommit( EntityID entity )
{

}

void Systems::TriggerSystem::OnEntityRemoved( EntityID entity )
{

}

bool Systems::TriggerSystem::OnEnterTrigger( const Events::EnterTrigger &event )
{

	auto teamComponent = m_World->GetComponent<Components::Team>(event.Trigger);
	auto playerComponent = m_World->GetComponent<Components::Player>(event.Entity);

	if(!teamComponent || !playerComponent)
		return false;
	if (teamComponent->TeamID != playerComponent->ID)
		return false;

	auto flagComponent1 = m_World->GetComponent<Components::Flag>(event.Trigger);
	auto flagComponent2 = m_World->GetComponent<Components::Flag>(event.Entity);

	 //HACK: SIMON IS DISSING AMERICA
	if(flagComponent1)
	{
		Flag(event.Entity, event.Trigger);
	}
	else if (flagComponent2)
	{
		Flag(event.Trigger, event.Entity);
	}


	auto triggerMoveComponent = m_World->GetComponent<Components::TriggerMove>(event.Trigger);
	if (triggerMoveComponent)
	{
		Move(triggerMoveComponent->Entity, triggerMoveComponent->Queue, triggerMoveComponent->Swap);
	}

	auto triggerRotateComponent = m_World->GetComponent<Components::TriggerRotate>(event.Trigger);
	if (triggerRotateComponent)
	{
		Rotate(triggerMoveComponent->Entity, triggerMoveComponent->Queue, triggerMoveComponent->Swap);
	}

	return true;
}


bool Systems::TriggerSystem::OnLeaveTrigger( const Events::LeaveTrigger &event )
{
	auto teamComponent = m_World->GetComponent<Components::Team>(event.Trigger);
	auto playerComponent = m_World->GetComponent<Components::Player>(event.Entity);

	if(!teamComponent || !playerComponent)
		return false;
	if (teamComponent->TeamID != playerComponent->ID)
		return false;

	auto flagComponent1 = m_World->GetComponent<Components::Flag>(event.Trigger);
	auto flagComponent2 = m_World->GetComponent<Components::Flag>(event.Entity);

	auto triggerMoveComponent = m_World->GetComponent<Components::TriggerMove>(event.Trigger);
	if (triggerMoveComponent)
	{
		Move(triggerMoveComponent->Entity, triggerMoveComponent->Queue, triggerMoveComponent->Swap);
	}

	auto triggerRotateComponent = m_World->GetComponent<Components::TriggerRotate>(event.Trigger);
	if (triggerRotateComponent)
	{
		Rotate(triggerMoveComponent->Entity, triggerMoveComponent->Queue, triggerMoveComponent->Swap);
	}

	return true;
}


void Systems::TriggerSystem::Rotate(EntityID entity, bool queue, bool swap)
{
	auto transform = m_World->GetComponent<Components::Transform>(entity);
	auto rotate = m_World->GetComponent<Components::Rotate>(entity);

	Events::Rotate e;
	e.Entity = entity;
	e.GoalRotation = rotate->GoalRotation;
	e.Time = rotate->Time;
	e.Queue = queue;
	EventBroker->Publish(e);

	if (swap)
	{
		std::swap(rotate->GoalRotation, rotate->StartRotation);
	}
}



void Systems::TriggerSystem::Move(EntityID entity, bool queue, bool swap)
{
	auto move = m_World->GetComponent<Components::Move>(entity);
	Events::Move e;
	e.Entity = entity;
	e.GoalPosition = move->GoalPosition;
	e.Speed = move->Speed;
	e.Queue = queue;
	EventBroker->Publish(e);

	if (swap)
	{
		std::swap(move->GoalPosition, move->StartPosition);
	}
}


void Systems::TriggerSystem::Flag( EntityID entity, EntityID phantomEntity )
{
	auto tankSteering = m_World->GetComponent<Components::TankSteering>(entity);
	if (!tankSteering)
		return;

	m_World->RemoveComponent<Components::Trigger>(phantomEntity);
	m_World->SetEntityParent(phantomEntity, entity);
	auto phantomTransform = m_World->GetComponent<Components::Transform>(phantomEntity);
	phantomTransform->Position = glm::vec3(-1.5f, 0.1f, 2.f);
}


void Systems::TriggerSystem::Explosion( EntityID entity, EntityID phantomEntity )
{
	/*auto transformComponent = m_World->GetComponent<Components::Transform>(entity);
	auto PhantomTransformComponent = m_World->GetComponent<Components::Transform>(phantomEntity);
	auto explosionComponent = m_World->GetComponent<Components::TriggerExplosion>(phantomEntity);

	if(transformComponent && PhantomTransformComponent)
	{
		// Velocity = (1 - (distance / radius)^2) * Strength;
		glm::vec3 vect = transformComponent->Position - PhantomTransformComponent->Position;
		float distance = glm::length(vect);
		float radius = explosionComponent->Radius;
		float velocity = (1.f - pow(distance / radius, 2)) * explosionComponent->MaxVelocity;

		glm::vec3 direction = glm::normalize(transformComponent->Position - PhantomTransformComponent->Position);

		Events::SetVelocity e;
		e.Entity = entity;
		e.Velocity = direction*velocity;
		EventBroker->Publish(e);

		m_World->RemoveEntity(phantomEntity);
	}*/
	
	
}



