#ifndef Components_TowerDebris_h__
#define Components_TowerDebris_h__

#include "Component.h"
#include <string>
#include <vector>

namespace Components
{

	struct TowerDebris : Component
	{
		std::vector<EntityID> TowerDebrisIDs;

		virtual TowerDebris* Clone() const override { return new TowerDebris(*this); }
	};

}

#endif // Components_TowerDebris_h__
