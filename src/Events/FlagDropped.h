#ifndef Events_FlagDropped_h__
#define Events_FlagDropped_h__

#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct FlagDropped : Event
	{
		EntityID Player;
	};
}

#endif // Events_FlagDropped_h__