#ifndef Components_Wall_h__
#define Components_Wall_h__

#include "Component.h"
#include <string>
#include <vector>

namespace Components
{

	struct Wall : Component
	{
		std::vector<EntityID> Walldebris;

		virtual Wall* Clone() const override { return new Wall(*this); }
	};

}

#endif // Components_TankShell_h__
