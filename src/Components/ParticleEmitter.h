#ifndef Components_ParticleEmitter_h__
#define Components_ParticleEmitter_h__

#include "Component.h"
#include "Color.h"
#include <vector>

namespace Components
{

struct ParticleEmitter : Component
{
	EntityID ParticleTemplate;
	float SpawnFrequency;
	int SpawnCount;
	std::vector<Color> ColorSpectrum;
	std::vector<float> ScaleSpectrum;
	float SpreadAngle;
	double LifeTime;
	std::vector<glm::vec3> VelocitySpectrum;
	std::vector<glm::vec3> AngularVelocitySpectrum;
};

}
#endif // !Components_ParticleEmitter_h__