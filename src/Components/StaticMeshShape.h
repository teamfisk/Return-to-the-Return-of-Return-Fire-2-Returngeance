#ifndef Components_StaticMeshShape_h__
#define Components_StaticMeshShape_h__

#include "Component.h"

namespace Components
{
	struct StaticMeshShape : Component
	{
		std::string Filename;
	};
}

#endif // Components_StaticMeshShape_h__
