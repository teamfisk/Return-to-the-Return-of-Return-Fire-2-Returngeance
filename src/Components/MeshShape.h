#ifndef Components_MeshShape_h__
#define Components_MeshShape_h__

#include <string>

#include "Component.h"

namespace Components
{

struct MeshShape : Component
{
	std::string ResourceName;
};

}
#endif // !Components_MeshShape_h__