#ifndef Components_Follow_h__
#define Components_Follow_h__

#include "Component.h"
#include "Entity.h"

namespace Components
{

	struct Follow : Component
	{
		EntityID Entity;
		glm::vec3 FollowAxis;
		float Distance;

		virtual Follow* Clone() const override { return new Follow(*this); }
	};

}
#endif // !Components_Follow_h__