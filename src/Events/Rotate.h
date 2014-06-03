#ifndef Event_Rotate_h__
#define Event_Rotate_h__

#include "EventBroker.h"

namespace Events
{

	struct Rotate : Event
	{
		EntityID Entity;
		glm::quat GoalRotation;
		double Time;
		bool Queue;
	};

}

#endif // Event_Rotate_h__
