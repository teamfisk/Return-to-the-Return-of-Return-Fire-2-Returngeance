#ifndef Components_Sphere_h__
#define Components_Sphere_h__

#include "Component.h"

namespace Components
{

struct SphereShape : Component
{
	SphereShape()
		: Radius(1.f){ }

	float Radius;
};

}

#endif // Components_Sphere_h__
