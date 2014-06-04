#ifndef Event_PlaySFX_h__
#define Event_PlaySFX_h__

#include "EventBroker.h"
#include "Entity.h"

namespace Events
{

struct PlaySFX : Event
{
	PlaySFX() : 
		MinDistance(100.f),
		MaxDistance(10000.f),
		Volume(1.f),
		Loop (false) {}
	//EntityID Emitter;
	glm::vec3 Position;
	std::string Resource;
	float MinDistance;
	float MaxDistance;
	float Volume;
	bool Loop;
};

}

#endif // Event_PlaySFX_h__
