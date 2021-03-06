#ifndef Events_BindKey_h__
#define Events_BindKey_h__

#include <boost/any.hpp>

#include "EventBroker.h"

namespace Events
{

struct BindKey : Event
{
	int KeyCode;
	std::string Command;
	float Value;
};

}

#endif // Events_BindKey_h__