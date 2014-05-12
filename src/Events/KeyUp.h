#ifndef Events_KeyUp_h__
#define Events_KeyUp_h__

#include "EventBroker.h"

namespace Events
{

struct KeyUp : Event
{
	int KeyCode;
};

}

#endif // Events_KeyUp_h__