#ifndef Events_TankSteer_h__
#define Events_TankSteer_h__
#include "Entity.h"
#include "EventBroker.h"

namespace Events
{

struct TankSteer : Event
{
	EntityID Entity;
	float PositionX;
	float PositionY;
	bool Handbrake;
};

}

#endif // Events_TankSteer_h__