#ifndef JeepSteering_h__
#define JeepSteering_h__

#include "Component.h"

namespace Components
{
	struct JeepSteering : Component
	{
		JeepSteering* Clone() const override { return new JeepSteering(*this); }
	};
}

#endif // JeepSteering_h__