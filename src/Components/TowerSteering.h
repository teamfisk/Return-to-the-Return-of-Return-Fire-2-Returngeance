#ifndef TowerSteering_h__
#define TowerSteering_h__

#include "Component.h"

	namespace Components
{

struct TowerSteering : Component
{
	TowerSteering()
		: Velocity(1.f), Axis(glm::vec3(0,1,0)){ }

	float Velocity;
	glm::vec3 Axis;
};

}

#endif // TowerSteering_h__