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

	ParticleEmitter() 
		: SpawnFrequency(0)
		, SpawnCount(0)
		, SpreadAngle(0)
		, LifeTime(0)
		, TimeSinceLastSpawn(100) { } // TEMP fulhack så att partiklarna spawnar direkt

	EntityID ParticleTemplate;
	float SpawnFrequency;
	float Speed;
	int SpawnCount;
	std::vector<Color> ColorSpectrum;
	std::vector<glm::vec3> ScaleSpectrum;
	float SpreadAngle;
	double LifeTime;
	bool UseGoalVelocity;
	glm::vec3 GoalVelocity;
	std::vector<float> AngularVelocitySpectrum;
	std::vector<glm::vec3> OrientationSpectrum; //Keep?

private:
	double TimeSinceLastSpawn;

	virtual ParticleEmitter* Clone() const override { return new ParticleEmitter(*this); }

};

}
#endif // !Components_ParticleEmitter_h__