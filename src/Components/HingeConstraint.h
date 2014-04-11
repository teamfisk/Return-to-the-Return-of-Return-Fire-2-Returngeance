#ifndef Components_HingeConstraint_h__
#define Components_HingeConstraint_h__

#include "Component.h"

namespace Components
{
// http://bulletphysics.org/mediawiki-1.5.8/index.php/Constraints
struct HingeConstraint : Component
{
	// Create constraint between these entities
	EntityID EntityA;
	EntityID EntityB;
	// The pivot point in local coordinates
	glm::vec3 PivotA;
	glm::vec3 PivotB;
	// The axis to rotate around
	glm::vec3 AxisA;
	glm::vec3 AxisB;

	/*
		void setLimit(btScalar low,
					  btScalar high,
					  btScalar softness = 0.9f,
					  btScalar_biasFactor = 0.3f,
					  btScalar relaxationFactor = 1.0f)
	*/
	float LowLimit;
	float HighLimit;
	float Softness = 0.9f;
	float BiasFactor = 0.3f;
	float RelaxationFactor = 1.0f;
};
}
#endif // Components_HingeConstraint_h__