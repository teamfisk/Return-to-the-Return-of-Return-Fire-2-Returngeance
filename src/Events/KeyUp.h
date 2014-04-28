#ifndef Event_KeyUp_h__
#define Event_KeyUp_h__

#include "EventBroker.h"

namespace Events
{

struct KeyUp : Event
{
	int KeyCode;
};

}

#endif // Event_KeyUp_h__