#ifndef BarrelSteering_h__
#define BarrelSteering_h__

#include "Component.h"

namespace Components
{

	struct BarrelSteering : Component
	{
		BarrelSteering()
			: TurnSpeed(1.f), Axis(glm::vec3(0,1,0)){ }
		float TurnSpeed;
		glm::vec3 Axis;
		EntityID ShotTemplate;
		float ShotSpeed;

		float LowerRotationLimit;
		float UpperRotationLimit;

		virtual BarrelSteering* Clone() const override { return new BarrelSteering(*this); }
	};

}

#endif // BarrelSteering_h__