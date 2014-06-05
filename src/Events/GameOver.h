#ifndef Events_GameOver_h__
#define Events_GameOver_h__

#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct GameOver : Event
	{
		unsigned int Player;
	};
}

#endif // Events_GameOver_h__