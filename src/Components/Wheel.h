#ifndef Components_Wheel_h__
#define Components_Wheel_h__

#include "Component.h"

namespace Systems { class PhysicsSystem; }

namespace Components
{

struct Wheel : Component
{	
	friend class Systems::PhysicsSystem;

	Wheel()
		: AxleID(0), Radius(0), Width(0), Mass(0), Steering(false), DownDirection(glm::vec3(0, -1, 0)), Friction(1.5f), SlipAngle(0.0f),
	MaxBreakingTorque(1500.0f), ConnectedToHandbrake(false), SuspensionStrength(50.0f) { }

	// The Hardpoint MUST be positioned INSIDE the chassis.
	glm::vec3 Hardpoint;
	unsigned int AxleID;
	float Radius;
	float Width;
	float Mass;
	bool Steering;
	glm::vec3 DownDirection;

	float SuspensionStrength;
	float Friction;
	float SlipAngle;
	float MaxBreakingTorque;
	bool ConnectedToHandbrake;

private:
	int ID;
	glm::quat OriginalOrientation;
};

}
#endif // Components_Wheel_h__
