#ifndef Components_Transform_h__
#define Components_Transform_h__

#include "Component.h"

namespace Components
{

struct Transform : Component
{
	Transform() 
		: Scale(glm::vec3(1.f)) { }

	glm::vec3 Position;
	glm::quat Orientation;
	glm::vec3 Velocity;
	glm::vec3 Scale;
};

}

#endif // Components_Transform_h__
