#ifndef Components_Stat_h__
#define Components_Stat_h__

#include "Component.h"

namespace Components
{

struct Stat : Component
{
	float Health;
	bool Destroyable;
};

}
#endif // !Components_Stat_h__