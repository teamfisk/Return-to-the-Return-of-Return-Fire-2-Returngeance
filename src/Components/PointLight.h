#ifndef Components_PointLight_h__
#define Components_PointLight_h__

#include "Component.h"
#include "Color.h"

namespace Components
{

struct PointLight : Component
{
	PointLight() 
		: Specular(1.0f, 1.0f, 1.0f)
		, Diffuse(0.4f, 0.4f, 0.4f)
		, specularExponent(50.0f)
		, ConstantAttenuation(1.05f)
		, LinearAttenuation(0.f)
		, QuadraticAttenuation(2.55f)
	{ }

	float ConstantAttenuation, LinearAttenuation, QuadraticAttenuation;
	Color color;

	glm::vec3 Specular;
	glm::vec3 Diffuse;
	float specularExponent;
	float Scale;
};

}
#endif // !Components_PointLight_h__