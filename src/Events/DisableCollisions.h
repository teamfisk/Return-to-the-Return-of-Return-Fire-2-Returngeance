#ifndef Events_DisableCollisions_h__
#define Events_DisableCollisions_h__
#include "Entity.h"
#include "EventBroker.h"

	namespace Events
{

	struct DisableCollisions : Event
	{
		int Layer1;
		int Layer2;
		
	};

}

#endif // Events_DisableCollisions_h__