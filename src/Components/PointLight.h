#ifndef Components_PointLight_h__
#define Components_PointLight_h__

#include "Component.h"
#include "Color.h"

namespace Components
{

struct PointLight : Component
{
	float Intensity;
	float MaxRange;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotExponent;
	Color color;

	glm::vec3 Specular;
	glm::vec3 Diffuse;
	float specularExponent;
};

}
#endif // !Components_PointLight_h__