#ifndef Event_PlaySFX_h__
#define Event_PlaySFX_h__

#include "EventBroker.h"
#include "Entity.h"

namespace Events
{

struct PlaySFX : Event
{
	EntityID Emitter;
	std::string Resource;
	bool Loop;
};

}

#endif // Event_PlaySFX_h__
