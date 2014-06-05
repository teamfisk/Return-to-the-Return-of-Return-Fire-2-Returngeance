#ifndef Events_FlagCaptured_h__
#define Events_FlagCaptured_h__

#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct FlagCaptured : Event
	{
		unsigned int Player;
	};
}

#endif // Events_FlagCaptured_h__