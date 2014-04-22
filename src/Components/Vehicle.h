#ifndef Components_Vehicle_h__
#define Components_Vehicle_h__

#include "Component.h"


namespace Components
{

struct Vehicle : Component
{
	Vehicle()
		: MaxTorque(500.0f), MinRPM(1000.0f), OptimalRPM(5500.0f), MaxRPM(7500.0f), MaxSteeringAngle(35), TopSpeed(50.0f) { }

	float MaxTorque;
	float MinRPM;
	float OptimalRPM;
	float MaxRPM;
	// Degrees
	float MaxSteeringAngle;
	float TopSpeed;
};

}

#endif // Components_Vehicle_h__
