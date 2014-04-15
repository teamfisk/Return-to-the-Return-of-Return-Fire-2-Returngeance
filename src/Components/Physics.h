#ifndef Components_Physics_h__
#define Components_Physics_h__

#include "Component.h"

namespace Components
{

struct Physics : Component
{
	Physics()
		: Mass(0.f) { }

	float Mass;
};

}

#endif // Components_Physics_h__
