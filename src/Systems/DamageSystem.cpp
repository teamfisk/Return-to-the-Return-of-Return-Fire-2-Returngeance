#include "PrecompiledHeader.h"
#include "DamageSystem.h"
#include "World.h"

void Systems::DamageSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::Health>([]() { return new Components::Health(); });
}

void Systems::DamageSystem::Initialize()
{

	EVENT_SUBSCRIBE_MEMBER(m_EDamage, &Systems::DamageSystem::OnDamage);
}

bool Systems::DamageSystem::OnDamage( const Events::Damage &event )
{
	auto health = m_World->GetComponent<Components::Health>(event.Entity);
	health->health -= event.damage;
	LOG_INFO("Damaged entity %i, Health left: %f", event.Entity, health->health);
	return true;
}