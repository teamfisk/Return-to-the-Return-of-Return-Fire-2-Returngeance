#include "PrecompiledHeader.h"
#include "DamageSystem.h"
#include "World.h"
void Systems::DamageSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Health>([]() { return new Components::Health(); });
	cf->Register<Components::DamageModel>([]() { return new Components::DamageModel(); });
}

void Systems::DamageSystem::Initialize()
{

	EVENT_SUBSCRIBE_MEMBER(m_EDamage, &Systems::DamageSystem::OnDamage);
}

bool Systems::DamageSystem::OnDamage( const Events::Damage &event )
{
	if(!m_World->ValidEntity(event.Entity))
		return false;
	auto health = m_World->GetComponent<Components::Health>(event.Entity);
	health->Amount -= event.Amount;
	//if(health->Amount <= 0)
	//{
	//	Events::OnDead e;
	//	e.Entity = event.Entity;
	//	EventBroker->Publish(e);
	//}
	LOG_INFO("Damaged entity %i, Health left: %f", event.Entity, health->Amount);
	return true;
}