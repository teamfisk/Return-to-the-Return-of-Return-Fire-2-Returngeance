#ifndef Events_MouseMove_h__
#define Events_MouseMove_h__

#include "EventBroker.h"

namespace Events
{

struct MouseMove : Event
{
	double X, Y;
	double DeltaX, DeltaY;
};

}

#endif // Events_MouseMove_h__