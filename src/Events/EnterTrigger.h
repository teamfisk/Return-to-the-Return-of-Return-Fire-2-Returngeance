#ifndef Events_EnterTrigger_h__
#define Events_EnterTrigger_h__
#include "Entity.h"
#include "EventBroker.h"

namespace Events
{

	struct EnterTrigger : Event
	{
		EntityID Entity1;
		EntityID Entity2;
	};

}

#endif // Events_EnterTrigger_h__