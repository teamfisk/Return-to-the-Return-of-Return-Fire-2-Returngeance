#include "PrecompiledHeader.h"
#include "WallSystem.h"
#include "World.h"

void Systems::WallSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Wall>([]() { return new Components::Wall(); });
}

void Systems::WallSystem::Initialize()
{
	EVENT_SUBSCRIBE_MEMBER(m_eDamage, &Systems::WallSystem::Damage);
}

bool Systems::WallSystem::Damage( const Events::Damage &event )
{
	auto wallComponent = m_World->GetComponent<Components::Wall>(event.Entity);
	if(!wallComponent)
	{
		return false;
	}
	LOG_INFO("WallSystem::Damage");
	auto wallhealthcomponent = m_World->GetComponent<Components::Health>(event.Entity);
	if(wallhealthcomponent->Amount > 0)
	{
		return false;
	}
	LOG_INFO("Entity %i is dead", event.Entity);
	//auto transformComponent = m_World->GetComponent<Components::Transform>(event.Entity);
	Components::Transform transformComponent = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(event.Entity);

	for(auto d : wallComponent->Walldebris)
	{
		auto debris = m_World->CloneEntity(d);
		
		auto transform = m_World->GetComponent<Components::Transform>(debris);
		transform->Orientation = transform->Orientation * transformComponent.Orientation;
		transform->Position = transform->Orientation * transform->Position + transformComponent.Position;
		
		//DO STUFF! :D
		float distance = glm::distance(transform->Position, transformComponent.Position);
		float radius = 5.f;
		float strength = (1.f - pow(distance / radius, 2)) * 20.f;
		glm::vec3 direction = glm::normalize(transformComponent.Position - transform->Position);

		Events::ApplyPointImpulse e;
		e.Entity = debris;
		e.Impulse = direction * strength;
		e.Position = transformComponent.Position;
		EventBroker->Publish(e);
	}

	m_World->RemoveEntity(event.Entity);
	return true;
}