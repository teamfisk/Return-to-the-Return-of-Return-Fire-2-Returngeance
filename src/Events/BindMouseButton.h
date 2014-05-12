#ifndef Events_BindMouseButton_h__
#define Events_BindMouseButton_h__

#include "EventBroker.h"

namespace Events
{

struct BindMouseButton : Event
{
	int Button;
	std::string Command;
};

}

#endif // Events_BindMouseButton_h__