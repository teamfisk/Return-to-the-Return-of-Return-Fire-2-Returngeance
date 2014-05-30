#ifndef TriggerExplosion_h__
#define TriggerExplosion_h__

#include "Component.h"

namespace Components
{

	struct TriggerExplosion : Component
	{
		TriggerExplosion()
			: MaxVelocity(1.f), Radius(1.f){ }

		// Velocity = (1 - (distance / radius)^2) * Strength;
		float MaxVelocity;
		float Radius; //HACK: Radius should only be in the SphereShapeComponent
		virtual TriggerExplosion* Clone() const override { return new TriggerExplosion(*this); }
	};

}

#endif // TriggerExplosion_h__