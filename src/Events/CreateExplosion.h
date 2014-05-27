#ifndef Event_CreateExplosion_h__
#define Event_CreateExplosion_h__

#include "EventBroker.h"
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Events
{
	struct CreateExplosion : Event
	{
		glm::vec3 Position;
		double LifeTime;
		int ParticlesToSpawn;
		std::string spritePath;
		glm::quat RelativeUpOrientation;
		float Speed;
		float SpreadAngle;
		float ParticleScale;
	};

}

#endif // Event_CreateExplosion_h__
