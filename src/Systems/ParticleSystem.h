#ifndef ParticleSystem_h__
#define ParticleSystem_h__

#include "System.h"
#include "Components/Transform.h"
#include "Components/ParticleEmitter.h"


namespace Systems
{


class ParticleSystem : public System
{
public:
	ParticleSystem(World* world);
	void RegisterComponents(ComponentFactory* cf) override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

private:
	void SpawnParticles();


	
};


}


#endif // ParticleSystem_h__