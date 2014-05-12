#ifndef Components_MeshShape_h__
#define Components_MeshShape_h__

#include <string>

#include "Component.h"

namespace Components
{

struct MeshShape : Component
{
	std::string ResourceName;

	virtual MeshShape* Clone() const override { return new MeshShape(*this); }
};

}
#endif // !Components_MeshShape_h__