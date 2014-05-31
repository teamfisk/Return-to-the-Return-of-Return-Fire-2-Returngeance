#ifndef Components_Particle_h__
#define Components_Particle_h__

#include "System.h"
#include "Component.h"
#include "Color.h"
#include <vector>

namespace Components
{

	struct Particle : Component
	{
		std::vector<glm::vec3> ScaleSpectrum;
		double LifeTime;
		double SpawnTime;
		bool Fade;
		std::vector<glm::vec3> VelocitySpectrum;
		std::vector<float> AngularVelocitySpectrum;
		std::vector<glm::vec3> OrientationSpectrum; //Keep?

		virtual Particle* Clone() const override { return new Particle(*this); }
	};

}
#endif // !Components_Particle_h__