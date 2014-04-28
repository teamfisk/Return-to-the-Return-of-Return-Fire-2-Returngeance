#ifndef Components_Vehicle_h__
#define Components_Vehicle_h__

#include "Component.h"


namespace Components
{

struct Vehicle : Component
{
	Vehicle()
		: MaxTorque(1000.0f), MinRPM(1000.0f), OptimalRPM(3000.0f), MaxRPM(4000.0f), MaxSteeringAngle(35), TopSpeed(130.0f),
	MaxSpeedFullSteeringAngle(40.0f){ }

	float MaxTorque;
	float MinRPM;
	float OptimalRPM;
	float MaxRPM;
	// Degrees
	float MaxSteeringAngle;
	float TopSpeed;
	float MaxSpeedFullSteeringAngle;
};

}

#endif // Components_Vehicle_h__
