#ifndef HelicopterSteering_h__
#define HelicopterSteering_h__

#include "Component.h"

namespace Components
{

struct HelicopterSteering : Component
{
	HelicopterSteering* Clone() const override { return new HelicopterSteering(*this); }
};

}

#endif // HelicopterSteering_h__