#ifndef Components_Box_h__
#define Components_Box_h__

#include "Component.h"

namespace Components
{

struct BoxShape : Component
{
	BoxShape()
		: Width(1.f), Height(1.f), Depth(1.f){ }

	float Width;
	float Height;
	float Depth;
};

}

#endif // Components_Box_h__
