#ifndef Components_Transform_h__
#define Components_Transform_h__

#include "Component.h"

namespace Components
{

struct Transform : public Component
{
	Transform()
		: Scale(glm::vec3(1.f)) { }

	glm::vec3 Position;
	glm::quat Orientation;
	glm::vec3 Velocity;
	glm::vec3 Scale;

	virtual Transform* Clone() const override { return new Transform(*this); }
};

}

#endif // Components_Transform_h__
