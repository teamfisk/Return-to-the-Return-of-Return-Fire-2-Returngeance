#ifndef Event_Move_h__
#define Event_Move_h__

#include "EventBroker.h"

namespace Events
{

	struct Move : Event
	{
		EntityID Entity;
		glm::vec3 GoalPosition;
		double Time;
		bool Queue;
	};

}

#endif // Event_Move_h__
