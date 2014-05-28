#ifndef Events_Damage_h__
#define Events_Damage_h__
#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct Damage : Event
	{
		EntityID Entity;
		float Amount;
	};
}



#endif // Events_Damage_h__