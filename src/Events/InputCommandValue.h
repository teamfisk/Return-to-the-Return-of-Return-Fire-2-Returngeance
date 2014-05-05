#ifndef Events_InputCommandValue_h__
#define Events_InputCommandValue_h__

#include "EventBroker.h"
#include "Events/InputCommand.h"

namespace Events
{

template <typename T>
struct InputCommandValue : public InputCommand
{
	T Value;
};

}

#endif // Events_InputCommandValue_h__