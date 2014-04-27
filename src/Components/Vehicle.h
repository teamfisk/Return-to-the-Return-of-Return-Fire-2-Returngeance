#ifndef Components_Vehicle_h__
#define Components_Vehicle_h__

#include "Component.h"


namespace Components
{

struct Vehicle : Component
{
	Vehicle()
		: MaxTorque(500.0f), MinRPM(800.0f), OptimalRPM(4000.0f), MaxRPM(6000.0f), MaxSteeringAngle(35), TopSpeed(50.0f) { }

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
