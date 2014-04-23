#ifndef ParticleSystem_h__
#define ParticleSystem_h__

#include "System.h"
#include "Components/Transform.h"
#include "Components/ParticleEmitter.h"
#include "Components/Particle.h"
#include "Components/Model.h"
#include "Color.h"
#include <GLFW/glfw3.h>

namespace Systems
{

	struct ParticleData
	{
		EntityID ParticleID;
		double SpawnTime;
		float Scale;
		Color color;
		glm::vec3 Velocity;
		glm::vec3 Direction;
	};

class ParticleSystem : public System
{
public:
	ParticleSystem(World* world);
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