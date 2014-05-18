#ifndef Components_Camera_h__
#define Components_Camera_h__

#include "Component.h"
#include "Entity.h"

namespace Components
{

struct Camera : Component
{
	Camera() 
	: FOV(glm::radians(45.f))
	, NearClip(0.1f)
	, FarClip(100.f) { }

	EntityID Viewport;
	float FOV;
	float NearClip;
	float FarClip;

	virtual Camera* Clone() const override { return new Camera(*this); }
};

}
#endif // !Components_Camera_h__