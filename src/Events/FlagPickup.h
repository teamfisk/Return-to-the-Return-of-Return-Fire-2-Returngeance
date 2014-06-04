#ifndef Events_FlagPickup_h__
#define Events_FlagPickup_h__

#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct FlagPickup : Event
	{
		EntityID Player;
	};
}

#endif // Events_FlagPickup_h__