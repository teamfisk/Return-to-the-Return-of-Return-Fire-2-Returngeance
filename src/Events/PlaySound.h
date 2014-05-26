#ifndef Event_PlaySound_h__
#define Event_PlaySound_h__

#include "EventBroker.h"
#include "Entity.h"

namespace Events
{

struct PlaySound : Event
{
	EntityID Emitter;
	std::string Resource;
	bool Loop;
};

}

#endif // Event_PlaySound_h__
