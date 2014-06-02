#ifndef Events_SpawnVehicle_h__
#define Events_SpawnVehicle_h__

#include "Entity.h"
#include "EventBroker.h"

namespace Events
{
	struct SpawnVehicle : Event
	{
		enum class VehicleType
		{
			Tank = 0,
			Helicopter,
			HRSV,
			Jeep
		};

		int PlayerID;
		VehicleType Vehicle;
	};
}

#endif // Events_SpawnVehicle_h__