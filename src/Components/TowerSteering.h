#ifndef TowerSteering_h__
#define TowerSteering_h__

#include "Component.h"

	namespace Components
{

struct TowerSteering : Component
{
	TowerSteering()
		: TurnSpeed(1.f), Axis(glm::vec3(0,1,0)){ }
	float TurnSpeed;
	glm::vec3 Axis;
	virtual TowerSteering* Clone() const override { return new TowerSteering(*this); }
};

}

#endif // TowerSteering_h__