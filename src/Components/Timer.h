#ifndef Components_Timer_h__
#define Components_Timer_h__

#include "Component.h"

namespace Components
{

	struct Timer : public Component
	{
		double Time;
		virtual Timer* Clone() const override { return new Timer(*this); }
	};

}

#endif // Components_Timer_h__
