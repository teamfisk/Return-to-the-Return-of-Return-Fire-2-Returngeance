#ifndef Components_BallSocketConstraint_h__
#define Components_BallSocketConstraint_h__

#include "Component.h"

namespace Components
{
// http://bulletphysics.org/mediawiki-1.5.8/index.php/Constraints
struct BallSocketConstraint : Component
{
	// Create constraint between these entities
	EntityID EntityA;
	EntityID EntityB;
	// The pivot point in local coordinates
	glm::vec3 PivotA;
	glm::vec3 PivotB;
};
}
#endif // Components_BallSocketConstraint_h__