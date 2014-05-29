#ifndef TriggerMove_h__
#define TriggerMove_h__

#include "Component.h"

namespace Components
{

	struct TriggerMove : Component
	{
		EntityID Entity;
		float Time;
		glm::vec3 GoalPosition;
		virtual TriggerMove* Clone() const override { return new TriggerMove(*this); }
	};

}

#endif // TriggerMove_h__