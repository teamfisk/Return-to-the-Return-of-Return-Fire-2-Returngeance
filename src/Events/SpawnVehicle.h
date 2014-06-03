#ifndef Events_SpawnVehicle_h__
#define Events_SpawnVehicle_h__

#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct SpawnVehicle : Event
	{
		int PlayerID;
		std::string VehicleType;
	};
}

#endif // Events_SpawnVehicle_h__