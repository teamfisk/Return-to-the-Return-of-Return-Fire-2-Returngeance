#ifndef Events_JeepSteer_h__
#define Events_JeepSteer_h__
#include "Entity.h"
#include "EventBroker.h"

namespace Events
{

	struct JeepSteer : Event
	{
		EntityID Entity;
		float PositionX;
		float PositionY;
		bool Handbrake;
	};

}

#endif // Events_JeepSteer_h__