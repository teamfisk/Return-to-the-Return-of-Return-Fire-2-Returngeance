#include "PrecompiledHeader.h"
#include "ParticleSystem.h"


#include "World.h"

void Systems::ParticleSystem::Update(double dt)
{
	m_TimeSinceLastSpawn += dt;
}

void Systems::ParticleSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if(!transformComponent)
		return;

	auto emitterComponent = m_World->GetComponent<Components::ParticleEmitter>(entity, "ParticleEmitter");
	if(emitterComponent)
	{
		if(m_TimeSinceLastSpawn > emitterComponent->SpawnFrequency)
		{
			SpawnParticles(entity, emitterComponent->SpawnCount, emitterComponent->SpreadAngle);
			m_TimeSinceLastSpawn = 0;
		}
		
		std::list<ParticleData>::iterator it;
		for(it = m_ParticleEmitter[entity].begin(); it == m_ParticleEmitter[entity].end();)
		{
			it->TimeLived += dt;
			auto particleComponent = m_World->GetComponent<Components::Particle>(it->ParticleID, "Particle");
			if(it->TimeLived > particleComponent->LifeTime)
			{
				m_ParticleEmitter[entity].erase(it);
				break;
			}
			else
			{
				it++;
			}


		}
	}
}

void Systems::ParticleSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("ParticleEmitter", []() { return new Components::ParticleEmitter(); });
	cf->Register("Particle", []() { return new Components::Particle(); });
}

void Systems::ParticleSystem::SpawnParticles(EntityID emitterID, float spawnCount, float spreadAngle)
{
	std::list<EntityID> particles;
	for(int i = 0; i < spawnCount; i++)
	{
		auto ent = m_World->CreateEntity();
		auto transform = m_World->AddComponent<Components::Transform>(ent, "Transform");
		transform->Position = glm::vec3(0);
		auto particle = m_World->AddComponent<Components::Particle>(ent, "Particle");
		particle->LifeTime = 4;
		ParticleData data;
		data.ParticleID = ent;
		data.TimeLived = 0;
		m_ParticleEmitter[emitterID].push_back(data);
	}
}

void Systems::ParticleSystem::Draw(double dt)
{

}


