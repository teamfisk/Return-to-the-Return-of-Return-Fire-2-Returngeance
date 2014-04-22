#ifndef ParticleSystem_h__
#define ParticleSystem_h__

#include "System.h"
#include "Components/Transform.h"
#include "Components/ParticleEmitter.h"
#include "Components/Particle.h"


namespace Systems
{

	struct ParticleData
	{
		EntityID ParticleID;
		double TimeLived;
	};

class ParticleSystem : public System
{
public:
	ParticleSystem(World* world)
		:System(world) { };
	void RegisterComponents(ComponentFactory* cf) override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	void Draw(double dt);
private:
	void SpawnParticles(EntityID emitterID, float spawnCount, float spreadAngle);
	std::map<EntityID, std::list<ParticleData>> m_ParticleEmitter;
	double m_TimeSinceLastSpawn;
	
};

}


#endif // !ParticleSystem_h__