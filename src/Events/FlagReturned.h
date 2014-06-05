#ifndef Events_FlagReturned_h__
#define Events_FlagReturned_h__

#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct FlagReturned : Event
	{
		EntityID Player;
	};
}

#endif // Events_FlagReturned_h__