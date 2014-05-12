#ifndef Events_KeyDown_h__
#define Events_KeyDown_h__

#include "EventBroker.h"

namespace Events
{

struct KeyDown : Event
{
	int KeyCode;
};

}

#endif // Events_KeyDown_h__