#ifndef Events_Dead_h__
#define Events_Dead_h__
#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct Dead : Event
	{
		EntityID Entity;
	};
}



#endif // Events_Dead_h__