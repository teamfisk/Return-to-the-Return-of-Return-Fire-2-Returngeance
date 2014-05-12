#ifndef Events_CastRay_h__
#define Events_CastRay_h__

#include "EventBroker.h"

namespace Events
{

struct CastRay : Event
{
	glm::vec3 Direction;
};

}

#endif // Events_CastRay_h__