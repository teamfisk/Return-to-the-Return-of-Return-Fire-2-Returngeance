#ifndef Events_MouseRelease_h__
#define Events_MouseRelease_h__

#include "EventBroker.h"

namespace Events
{

struct MouseRelease : Event
{
	int Button;
};

}

#endif // Events_MouseRelease_h__