#include "ParticleSystem.h"
#include "PrecompiledHeader.h"

#include "World.h"
void Systems::ParticleSystem::Update(double dt)
{

}

void Systems::ParticleSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if(!transformComponent)
		return;
	
	transformComponent->Position.y --;
	
	timeLived[entity] += dt;
	
	auto emitterComponent = m_World->GetComponent<Components::ParticleEmitter>(entity, "ParticleEmitter");
	
	

}

void Systems::ParticleSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("ParticleEmitter", []() { return new Components::ParticleEmitter(); });
}

void Systems::ParticleSystem::SpawnParticles()
{
	for(int i = 0; i < 100; i++)
	{
		auto particle = m_World->CreateEntity();
		auto transform = m_World->AddComponent<Components::Transform>(particle, "Transform");
		transform->Position = glm::vec3(0);
		particles.push_back(particle);
	}
}

void Systems::ParticleSystem::Draw(double dt)
{

}


