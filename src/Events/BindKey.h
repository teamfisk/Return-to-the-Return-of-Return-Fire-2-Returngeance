#ifndef Events_BindKey_h__
#define Events_BindKey_h__

#include "EventBroker.h"

namespace Events
{

struct BindKey : Event
{
	int KeyCode;
	std::string Command;
};

}

#endif // Events_BindKey_h__