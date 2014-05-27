#include "PrecompiledHeader.h"
#include "TriggerSystem.h"
#include "World.h"

void Systems::TriggerSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Trigger>([]() { return new Components::Trigger(); });
	cf->Register<Components::TriggerExplosion>([]() { return new Components::TriggerExplosion(); });
}

void Systems::TriggerSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_EEnterTrigger, &Systems::TriggerSystem::OnEnterTrigger);
}

void Systems::TriggerSystem::Update( double dt )
{

}

void Systems::TriggerSystem::UpdateEntity( double dt, EntityID entity, EntityID parent )
{

}

void Systems::TriggerSystem::OnComponentCreated( std::string type, std::shared_ptr<Component> component )
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
	/*auto explosionComponent1 = m_World->GetComponent<Components::TriggerExplosion>(event.Entity1);
	auto explosionComponent2 = m_World->GetComponent<Components::TriggerExplosion>(event.Entity2);

	if(explosionComponent1)
	{
		Explosion(event.Entity2, event.Entity1);
	}
	else if (explosionComponent2)
	{
		Explosion(event.Entity1, event.Entity2);
	}*/

	auto flagComponent1 = m_World->GetComponent<Components::Flag>(event.Entity1);
	auto flagComponent2 = m_World->GetComponent<Components::Flag>(event.Entity2);

	if(flagComponent1)
	{
		Flag(event.Entity2, event.Entity1);
	}
	else if (flagComponent2)
	{
		Flag(event.Entity1, event.Entity2);
	}

	return true;
}


void Systems::TriggerSystem::Flag( EntityID entity, EntityID phantomEntity )
{
	auto tankSteering = m_World->GetComponent<Components::TankSteering>(entity);
	if (!tankSteering)
		return;

	m_World->RemoveComponent<Components::Trigger>(phantomEntity);
	m_World->SetEntityParent(phantomEntity, entity);
	auto phantomTransform = m_World->GetComponent<Components::Transform>(phantomEntity);
	phantomTransform->Position = glm::vec3(1.f, 0.1f, 2.f);
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



