#ifndef TriggerRotate_h__
#define TriggerRotate_h__

#include "Component.h"

namespace Components
{

	struct TriggerRotate : Component
	{
		TriggerRotate()
			: Queue(false)
			, Swap(true)
		{ }

		EntityID Entity;
		bool Queue;
		bool Swap;
		virtual TriggerRotate* Clone() const override { return new TriggerRotate(*this); }
	};

}

#endif // TriggerRotate_h__