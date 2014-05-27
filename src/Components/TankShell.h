#ifndef Components_TankShell_h__
#define Components_TankShell_h__

#include "Component.h"

	namespace Components
{

	struct TankShell : Component
	{
		TankShell()
			: Damage(1.0f){ }

		float Damage;
		float ExplosionRadius;
		float ExplosionStrength;

		virtual TankShell* Clone() const override { return new TankShell(*this); }
	};

}

#endif // Components_TankShell_h__
