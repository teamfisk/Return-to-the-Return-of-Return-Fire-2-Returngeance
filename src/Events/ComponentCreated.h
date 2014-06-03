#ifndef Event_ComponentCreated_h__
#define Event_ComponentCreated_h__

#include "EventBroker.h"
#include "Entity.h"
#include "Component.h"

namespace Events
{

struct ComponentCreated : Event
{
	EntityID Entity;
	std::shared_ptr<::Component> Component;
};

}

#endif // Event_ComponentCreated_h__
