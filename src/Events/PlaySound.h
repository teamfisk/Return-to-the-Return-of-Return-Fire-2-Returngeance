#ifndef Event_PlaySound_h__
#define Event_PlaySound_h__

#include "EventBroker.h"

namespace Events
{

struct PlaySound : Event
{
	EntityID Emitter;
	std::string Resource;
};

}

#endif // Event_PlaySound_h__
