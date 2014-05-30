#ifndef Events_EnableCollisions_h__
#define Events_EnableCollisions_h__
#include "Entity.h"
#include "EventBroker.h"

namespace Events
{

	struct EnableCollisions : Event
	{
		int Layer1;
		int Layer2;

	};

}

#endif // Events_EnableCollisions_h__