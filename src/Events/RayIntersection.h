#ifndef Events_RayIntersection_h__
#define Events_RayIntersection_h__

#include "EventBroker.h"
#include "Entity.h"

namespace Events
{

struct RayIntersection : Event
{
	EntityID Entity;
};

}

#endif // Events_RayIntersection_h__