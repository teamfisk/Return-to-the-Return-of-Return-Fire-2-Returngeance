#ifndef Components_VehicleSpawn_h__
#define Components_VehicleSpawn_h__

#include "Component.h"

namespace Components
{

struct SpawnPoint : Component
{
	EntityID Player;

	virtual SpawnPoint* Clone() const override { return new SpawnPoint(*this); }
};

}

#endif // Components_VehicleSpawn_h__