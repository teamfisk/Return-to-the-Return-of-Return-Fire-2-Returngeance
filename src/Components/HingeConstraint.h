#ifndef Components_HingeConstraint_h__
#define Components_HingeConstraint_h__

#include "Component.h"

namespace Components
{

	struct HingeConstraint : Component
	{
		EntityID LinkedEntity;
		glm::vec3 Pivot;
		glm::vec3 Axis;
	};

}

#endif // Components_HingeConstraint_h__
