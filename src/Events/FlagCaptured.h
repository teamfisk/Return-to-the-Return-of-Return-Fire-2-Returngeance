#ifndef Events_FlagCaptured_h__
#define Events_FlagCaptured_h__

#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct FlagCaptured : Event
	{
		EntityID Player;
	};
}

#endif // Events_FlagCaptured_h__