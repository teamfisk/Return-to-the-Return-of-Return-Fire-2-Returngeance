#ifndef Components_Turret_h__
#define Components_Turret_h__

#include "Component.h"

namespace Components
{

	struct Turret : Component
	{
		Turret()
			: ShotSpeed(1.0f) { }

		EntityID ShotTemplate;
		float ShotSpeed;

		virtual Turret* Clone() const override { return new Turret(*this); }
	};

}

#endif // BarrelSteering_h__