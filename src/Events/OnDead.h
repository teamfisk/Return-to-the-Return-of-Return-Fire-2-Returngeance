#ifndef Events_OnDead_h__
#define Events_OnDead_h__
#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct OnDead : Event
	{
		EntityID Entity;
	};
}



#endif // Events_OnDead_h__