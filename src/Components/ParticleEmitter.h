#ifndef Components_ParticleEmitter_h__
#define Components_ParticleEmitter_h__

#include "Component.h"
#include "Color.h"
#include <vector>

namespace Systems { class ParticleSystem; }

namespace Components
{

struct ParticleEmitter : Component
{
	friend class Systems::ParticleSystem;

	EntityID ParticleTemplate;
	float SpawnFrequency;
	int SpawnCount;
	std::vector<Color> ColorSpectrum;
	std::vector<float> ScaleSpectrum;
	float SpreadAngle;
	double LifeTime;
	std::vector<glm::vec3> VelocitySpectrum;
	std::vector<glm::vec3> AngularVelocitySpectrum;

private:
	double TimeSinceLastSpawn;
};

}
#endif // !Components_ParticleEmitter_h__