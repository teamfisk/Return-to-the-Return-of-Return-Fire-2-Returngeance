#ifndef Components_TurretShot_h__
#define Components_TurretShot_h__

#include "Component.h"

namespace Components
{

	struct TurretShot : Component
	{
		TurretShot()
			: Damage(1.0f),
			  ExplosionRadius(1.0f),
			  ExplosionStrength(100.0f) { }

		float Damage;
		float ExplosionRadius;
		float ExplosionStrength;

		virtual TurretShot* Clone() const override { return new TurretShot(*this); }
	};

}

#endif // Components_TurretShot_h__
