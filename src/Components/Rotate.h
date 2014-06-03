#ifndef Components_Rotate_h__
#define Components_Rotate_h__

#include "Component.h"

namespace Components
{

struct Rotate : Component
{
	float Time;
	glm::quat StartRotation;
	glm::quat GoalRotation;
	virtual Rotate* Clone() const override { return new Rotate(*this); }
};

}

#endif // Components_Rotate_h__