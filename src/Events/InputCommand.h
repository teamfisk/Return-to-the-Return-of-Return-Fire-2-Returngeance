#ifndef Events_InputCommand_h__
#define Events_InputCommand_h__

#include <boost/any.hpp>

#include "EventBroker.h"

namespace Events
{

struct InputCommand : Event
{
	unsigned int PlayerID;
	std::string Command;
	boost::any Value;
};

}

#endif // Events_InputCommand_h__