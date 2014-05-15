#ifndef Components_WheelPair_h__
#define Components_WheelPair_h__

#include "Component.h"

namespace Components
{

	struct WheelPair : Component
	{
		// Flag for pair wheels

		virtual WheelPair* Clone() const override { return new WheelPair(*this); }
	};

}

#endif // Components_WheelPair_h__
