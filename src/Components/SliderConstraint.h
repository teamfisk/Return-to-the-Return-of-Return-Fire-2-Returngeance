#ifndef Components_SliderConstraint_h__
#define Components_SliderConstraint_h__

#include "Component.h"

namespace Components
{
	// http://bulletphysics.org/mediawiki-1.5.8/index.php/Constraints
	struct SliderConstraint : Component
	{
		// Create constraint between these entities
		EntityID EntityA;
		EntityID EntityB;
	};
}
#endif // Components_SliderConstraint_h__