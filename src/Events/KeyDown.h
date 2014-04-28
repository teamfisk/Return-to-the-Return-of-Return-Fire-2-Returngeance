#ifndef Event_KeyDown_h__
#define Event_KeyDown_h__

#include "EventBroker.h"

namespace Events
{

struct KeyDown : Event
{
	int KeyCode;
};

}

#endif // Event_KeyDown_h__
