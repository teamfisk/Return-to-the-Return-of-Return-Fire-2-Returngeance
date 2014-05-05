#ifndef Events_MousePress_h__
#define Events_MousePress_h__

#include "EventBroker.h"

namespace Events
{

struct MousePress : Event
{
	int Button;
};

}

#endif // Events_MousePress_h__