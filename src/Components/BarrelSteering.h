#ifndef BarrelSteering_h__
#define BarrelSteering_h__

#include "Component.h"

namespace Components
{

	struct BarrelSteering : Component
	{
		BarrelSteering()
			: Velocity(1.f), Axis(glm::vec3(0,1,0)){ }
		float Velocity;
		glm::vec3 Axis;
		EntityID ShotTemplate;
	};

}

#endif // BarrelSteering_h__