#ifndef Events_Collision_h__
#define Events_Collision_h__

#include "Entity.h"
#include "EventBroker.h"

namespace Events
{

struct Collision : Event
{
	EntityID Entity1;
	EntityID Entity2;
};

}

#endif // Events_Collision_h__