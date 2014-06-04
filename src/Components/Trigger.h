#ifndef Trigger_h__
#define Trigger_h__

#include "Component.h"

namespace Components
{

	struct Trigger : Component
	{
		bool TriggerOnce;
		int TeamID;
		virtual Trigger* Clone() const override { return new Trigger(*this); }
	};

}

#endif // Trigger_h__