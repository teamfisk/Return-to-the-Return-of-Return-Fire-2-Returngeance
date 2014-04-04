#ifndef Components_Camera_h__
#define Components_Camera_h__

#include "Component.h"

namespace Components
{

struct Camera : Component
{
	Camera() : FOV(glm::radians(45.f)), NearClip(0.1f), FarClip(100.f) { }

	float FOV;
	float NearClip;
	float FarClip;
};

}
#endif // !Components_Camera_h__