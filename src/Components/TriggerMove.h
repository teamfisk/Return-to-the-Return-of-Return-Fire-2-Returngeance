#ifndef TriggerMove_h__
#define TriggerMove_h__

#include "Component.h"

namespace Components
{

	struct TriggerMove : Component
	{
		TriggerMove()
			: Queue(false)
			, Swap(true)
		{ }

		EntityID Entity;
		bool Queue;
		bool Swap;
		virtual TriggerMove* Clone() const override { return new TriggerMove(*this); }
	};

}

#endif // TriggerMove_h__