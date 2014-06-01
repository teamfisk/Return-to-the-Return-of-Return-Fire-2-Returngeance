#ifndef Components_Garage_h__
#define Components_Garage_h__

#include "Component.h"

namespace Components
{
	struct Garage : Component
	{
		Garage()
			: Open(false)
			, Elevator(0)
			, DoorLeft(0)
			, DoorRight(0)
			, Light1(0)
			, Light2(0)
			, Light3(0)
			, Light4(0)
		{ }

		EntityID Elevator;
		EntityID DoorLeft;
		EntityID DoorRight;
		EntityID Light1;
		EntityID Light2;
		EntityID Light3;
		EntityID Light4;

		bool Open;

		virtual Garage* Clone() const override { return new Garage(*this); }
	};
}

#endif // Components_TankShell_h__
