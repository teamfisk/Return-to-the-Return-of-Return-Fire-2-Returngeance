#ifndef Components_Sphere_h__
#define Components_Sphere_h__

#include "Component.h"

namespace Components
{

struct Sphere : Component
{
	Sphere()
		: Radius(1.f){ }

	float Radius;

	virtual Sphere* Clone() const override { return new Sphere(*this); }
};

}

#endif // Components_Sphere_h__
