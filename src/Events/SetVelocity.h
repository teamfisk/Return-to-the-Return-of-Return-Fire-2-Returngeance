#ifndef Events_SetVelocity_h__
#define Events_SetVelocity_h__
#include "Entity.h"
#include "EventBroker.h"

namespace Events
{

	struct SetVelocity : Event
	{
		EntityID Entity;
		glm::vec3 Velocity;
	};

}

#endif // Events_SetVelocity_h__