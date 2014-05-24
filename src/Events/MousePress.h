#ifndef Events_MousePress_h__
#define Events_MousePress_h__

#include "EventBroker.h"

namespace Events
{

struct MousePress : Event
{
	int Button;
	double X, Y;
};

}

#endif // Events_MousePress_h__