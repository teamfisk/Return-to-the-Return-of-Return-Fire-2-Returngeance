#ifndef Components_Move_h__
#define Components_Move_h__

#include "Component.h"

namespace Components
{

struct Move : Component
{
	float Speed;
	glm::vec3 StartPosition;
	glm::vec3 GoalPosition;

	virtual Move* Clone() const override { return new Move(*this); }
};

}

#endif // Components_Move_h__