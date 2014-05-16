#ifndef Events_ApplyForce_h__
#define Events_ApplyForce_h__
#include "Entity.h"
#include "EventBroker.h"

namespace Events
{

struct ApplyForce : Event
{
	EntityID Entity;
	double DeltaTime;
	glm::vec3 Force;
};

}

#endif // Events_ApplyForce_h__