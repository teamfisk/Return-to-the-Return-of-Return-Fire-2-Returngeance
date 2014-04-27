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
		std::vector<Color> ColorSpectrum;
		std::vector<float> ScaleSpectrum;
		double LifeTime;
		std::vector<glm::vec3> VelocitySpectrum;
		std::vector<glm::vec3> AngularVelocitySpectrum;

		virtual Particle* Clone() const override { return new Particle(*this); }
	};

}
#endif // !Components_Particle_h__