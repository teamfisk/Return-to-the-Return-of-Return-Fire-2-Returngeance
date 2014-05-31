#ifndef TankSteering_h__
#define TankSteering_h__

#include "Component.h"

namespace Components
{
	struct TankSteering : Component
	{
		EntityID Turret;
		EntityID Barrel;
		TankSteering* Clone() const override { return new TankSteering(*this); }
	};
}

#endif // TankSteering_h__