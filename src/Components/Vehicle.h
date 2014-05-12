#ifndef Components_Vehicle_h__
#define Components_Vehicle_h__

#include "Component.h"


namespace Components
{

struct Vehicle : Component
{
	Vehicle()
		: MaxTorque(1000.0f), MinRPM(1000.0f), OptimalRPM(3000.0f), MaxRPM(4000.0f), MaxSteeringAngle(35), TopSpeed(130.0f),
	MaxSpeedFullSteeringAngle(40.0f), SpringDamping(1.f){ }

	float MaxTorque;
	float MinRPM;
	float OptimalRPM;
	float MaxRPM;
	// Degrees
	float MaxSteeringAngle;
	//TopSpeed not working fully yet 
	float TopSpeed;
	float MaxSpeedFullSteeringAngle;
	float SpringDamping;
};

}

#endif // Components_Vehicle_h__
