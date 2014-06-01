#ifndef Events_LeaveTrigger_h__
#define Events_LeaveTrigger_h__
#include "Entity.h"
#include "EventBroker.h"

	namespace Events
{

	struct LeaveTrigger : Event
	{
		EntityID Trigger;
		EntityID Entity;
	};

}

#endif // Events_LeaveTrigger_h__