#ifndef Components_BoxShape_h__
#define Components_BoxShape_h__

#include "Component.h"

namespace Components
{

struct BoxShape : Component
{
	float Height;
	float Width;
	float Depth;
};

}

#endif // !Components_BoxShape_h__
