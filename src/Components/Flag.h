#ifndef Components_Flag_h__
#define Components_Flag_h__

#include "Component.h"

namespace Components
{

	struct Flag : Component
	{

		virtual Flag* Clone() const override { return new Flag(*this); }
	};

}

#endif // Components_TankShell_h__
