#ifndef Components_Wheel_h__
#define Components_Wheel_h__

#include "Component.h"


namespace Components
{

	struct Wheel : Component
{
		EntityID CarID;

		glm::vec3 ConnectionPoint;
		glm::vec3 Direction;
		glm::vec3 Axle;
		float SuspensionRestLength;
		float Radius;
		bool IsFrontWheel;

		float 	SuspensionStiffness;
		float 	SuspensionCompression;
		float 	SuspensionDamping;
		float 	MaxSuspensionTravelCm;
		float 	FrictionSlip;
		float 	MaxSuspensionForce;
};

}

#endif // Components_Wheel_h__
