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
		, Diffuse(1.0f, 1.0f, 1.0f)
		, specularExponent(50.0f)
		, ConstantAttenuation(1.0f)
		, LinearAttenuation(0.f)
		, QuadraticAttenuation(3.f)
		, Radius(5.f)
	{ }

	float ConstantAttenuation, LinearAttenuation, QuadraticAttenuation;
	float Radius;
	Color color;

	glm::vec3 Specular;
	glm::vec3 Diffuse;
	float specularExponent;
	float Scale;
	
	virtual PointLight* Clone() const override { return new PointLight(*this); }
};

}
#endif // !Components_PointLight_h__