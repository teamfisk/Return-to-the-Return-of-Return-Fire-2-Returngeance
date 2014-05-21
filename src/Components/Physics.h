#ifndef Components_Physics_h__
#define Components_Physics_h__

#include "Component.h"

namespace Components
{

struct Physics : Component
{
	Physics()
		: Mass(1.f), Static(false), Phantom(false){}

	float Mass;
	bool Static;
	bool Phantom;

	virtual Physics* Clone() const override { return new Physics(*this); }
};

}

#endif // Components_Physics_h__
