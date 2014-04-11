#ifndef Components_SphereShape_h__
#define Components_SphereShape_h__

#include "Component.h"

namespace Components
{

struct SphereShape : Component
{	float Radius;
	float RollingFriction;
};

}

#endif // !Components_SphereShape_h__
