#ifndef Events_SetViewportCamera_h__
#define Events_SetViewportCamera_h__

#include "EventBroker.h"
#include "Entity.h"

namespace Events
{

struct SetViewportCamera : Event
{
	std::string ViewportFrame;
	EntityID CameraEntity;
};

}

#endif // Events_SetViewportCamera_h__
