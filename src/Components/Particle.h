#ifndef Components_Particle_h__
#define Components_Particle_h__

#include "System.h"
#include "Component.h"
#include "Components/Transform.h"
#include "Components/ParticleEmitter.h"
#include "Color.h"
#include <vector>

namespace Components
{

	struct Particle : Component
	{
		std::vector<Color> ColorSpectrum;
		std::vector<float> ScaleSpectrum;
		double LifeTime;
		std::vector<glm::vec3> VelocitySpectrum;
		std::vector<glm::vec3> AngularVelocitySpectrum;
	};

}
#endif // !Components_Particle_h__