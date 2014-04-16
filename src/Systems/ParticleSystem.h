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
	ParticleSystem(World* world)
		:System(world) { };
	void RegisterComponents(ComponentFactory* cf) override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	void Draw(double dt);
private:
	void SpawnParticles();
	
};


}


#endif // ParticleSystem_h__