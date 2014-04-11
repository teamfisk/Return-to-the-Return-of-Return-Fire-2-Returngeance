#ifndef Components_Bounds_h__
#define Components_Bounds_h__

#include "Component.h"

namespace Components
{

struct Bounds : Component
{	//Axis Aligned Bounding Box
	glm::vec3 Origin;
	glm::vec3 VolumeVector; //The vector that defines the volume of the BB, it goes from one corner to the opposite one
};

}
#endif // !Components_Bounds_h__