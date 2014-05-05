#ifndef Events_InputCommand_h__
#define Events_InputCommand_h__

#include "EventBroker.h"

namespace Events
{

struct InputCommand : Event
{
	unsigned int PlayerID;
	std::string Command;
};

}

#endif // Events_InputCommand_h__