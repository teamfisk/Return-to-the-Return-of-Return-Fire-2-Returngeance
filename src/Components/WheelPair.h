#ifndef Components_WheelPair_h__
#define Components_WheelPair_h__

#include "Component.h"

namespace Components
{

	struct WheelPair : Component
	{
		EntityID FakeWheel1;
		EntityID FakeWheel2;

		virtual WheelPair* Clone() const override { return new WheelPair(*this); }
	};

}

#endif // Components_WheelPair_h__
