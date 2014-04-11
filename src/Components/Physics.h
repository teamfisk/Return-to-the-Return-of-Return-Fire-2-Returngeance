#ifndef Components_Physics_h__
#define Components_Physics_h__

#include "Component.h"

namespace Components
{

struct Physics : Component
{	float Mass			= 0;
	float Friction		= 0;
	glm::vec3 Gravity = glm::vec3(0, -9.82f, 0);
};

}

#endif // !Components_Physics_h__
